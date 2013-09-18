package cl.telematic.business;

import cl.telematic.dao.UserDAO;
import cl.telematic.model.User;
import cl.telematic.model.UserRole;
import cl.telematic.security.Roles;
import org.apache.commons.codec.digest.DigestUtils;
import org.apache.commons.lang.RandomStringUtils;

import javax.annotation.Nonnull;
import javax.annotation.Resource;
import javax.annotation.security.PermitAll;
import javax.annotation.security.RolesAllowed;
import javax.ejb.EJB;
import javax.ejb.SessionContext;
import javax.ejb.Stateless;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import java.util.Date;
import java.util.List;

@RolesAllowed(Roles.USER)
@Stateless
public class UserManager {

    @Resource(mappedName = "java:jboss/mail/Default")
    private Session session;

    @Resource
    private SessionContext sessionContext;

    @EJB
    private UserDAO userDAO;

    //    @RolesAllowed(Roles.ADMIN)
    @Nonnull
    public List<User> getAllUsersOrderedByName()
    {
        return userDAO.getAllUsersOrderedByName();
    }

    @Nonnull
    public User getAuthenticatedUser()
    {
        final String principalName = sessionContext.getCallerPrincipal().getName();
        final User user = userDAO.findByEmail(principalName);
        if (null == user) {
            throw new IllegalStateException("Cannot find user by principal name: " + principalName);
        }
        return user;
    }

    @Nonnull
    public User changePassword(@Nonnull String currentPassword, @Nonnull String newPassword) throws InvalidCredentialsException
    {
        final User user = getAuthenticatedUser();
        final String dbPassword = user.getPassword();
        if (dbPassword == null || !dbPassword.equals(hashPassword(currentPassword))) {
            throw new InvalidCredentialsException("Wrong credentials");
        }
        user.setPassword(hashPassword(newPassword));
        return user;
    }

    //    @RolesAllowed(Roles.ADMIN)
    @Nonnull
    public User get(@Nonnull Long id)
    {
        return userDAO.get(id);
    }

    @PermitAll
    public boolean isEmailRegistered(@Nonnull String email)
    {
        return null != userDAO.findByEmail(email);
    }

    @PermitAll
    @Nonnull
    public User register(@Nonnull User user) throws EmailAleadyRegisteredException, MessagingException
    {
        final String email = user.getEmail();
        if (null == userDAO.findByEmail(email)) {
            user.setRole(UserRole.USER);
            final String password = RandomStringUtils.randomAlphanumeric(6);
            user.setPassword(hashPassword(password));
            sendRegistrationEmail(user, password);
            return userDAO.save(user);
        } else {
            throw new EmailAleadyRegisteredException("Email already registered: " + email);
        }
    }

    @Nonnull
    public User update(@Nonnull Long id, @Nonnull User user)
    {
        final User db = get(id);
        db.setName(user.getName());
        db.setPhoneNumber(user.getPhoneNumber());
        userDAO.save(db);
        return db;
    }

    private String hashPassword(String password)
    {
        return DigestUtils.sha512Hex(password);
    }

    private void sendRegistrationEmail(User user, String password) throws MessagingException
    {
        Message message = new MimeMessage(session);
        message.setRecipients(Message.RecipientType.TO, InternetAddress.parse(user.getEmail(), false));
        message.setSubject("Welcome to Electrical");
        String messageText = "Welcome, your password is:\n" + password;
        message.setText(messageText);
        message.setSentDate(new Date());
        Transport.send(message);
    }
}
