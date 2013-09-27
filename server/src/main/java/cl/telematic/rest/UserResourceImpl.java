package cl.telematic.rest;

import cl.telematic.business.InvalidCredentialsException;
import cl.telematic.business.UserManager;
import cl.telematic.rest.domain.User;
import cl.telematic.security.Authenticated;

import javax.annotation.Nonnull;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.inject.Inject;
import javax.validation.Validator;
import javax.validation.constraints.NotNull;
import java.util.List;

/**
 * JAX-RS Example
 * <p/>
 * This class produces a RESTful service to read/write the contents of the users table.
 */
@Stateless
public class UserResourceImpl implements UserResource {

    @EJB
    private DomainConverter domainConverter;

    @EJB
    private UserManager userManager;

    @Inject
    private Validator validator;

    @Override
    @Authenticated
    public List<User> getAllUsers()
    {
        return domainConverter.toUserList(userManager.getAllUsersOrderedByName());
    }

    @Override
    public User getAuthenticatedUser()
    {
        return domainConverter.toRestDomain(userManager.getAuthenticatedUser());
    }

    @Authenticated
    @Override
    public User getUser(@Nonnull Long id)
    {
        return domainConverter.toRestDomain(userManager.get(id));
    }

    @Authenticated
    @Override
    public User changePassword(@NotNull String currentPassword, @NotNull String newPassword)
    {
        if (null == currentPassword || null == newPassword) {
            throw new InvalidCredentialsException("Current password and new password are required");
        }
        return domainConverter.toRestDomain(userManager.changePassword(currentPassword, newPassword));
    }

    /**
     * Checks if a user with the same email address is already registered. This is the only way to easily capture the
     * "@UniqueConstraint(columnNames = "email")" constraint from the User class.
     *
     * @param email The email to check
     *
     * @return True if the email already exists, and false otherwise
     */
    @Override
    public boolean isEmailRegistered(String email)
    {
        return userManager.isEmailRegistered(email);
    }

    /**
     * Creates a new user from the values provided. Performs validation, and will return a JAX-RS response with either 200 ok,
     * or with a map of fields, and related errors.
     */
    @Authenticated
    @Override
    public User register(User user)
    {
        return domainConverter.toRestDomain(userManager.register(domainConverter.toDBDomain(user)));
    }

    @Authenticated
    @Override
    public User update(@Nonnull Long id, @Nonnull User user)
    {
        return domainConverter.toRestDomain(userManager.update(id, domainConverter.toDBDomain(user)));
    }
}
