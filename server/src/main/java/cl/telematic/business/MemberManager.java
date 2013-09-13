package cl.telematic.business;

import cl.telematic.dao.MemberDAO;
import cl.telematic.model.Member;
import cl.telematic.security.Roles;

import javax.annotation.Nonnull;
import javax.annotation.Resource;
import javax.annotation.security.PermitAll;
import javax.annotation.security.RolesAllowed;
import javax.ejb.EJB;
import javax.ejb.SessionContext;
import javax.ejb.Stateless;
import javax.persistence.NoResultException;

@RolesAllowed(Roles.USER)
@Stateless
public class MemberManager {

    @EJB
    private MemberDAO memberDAO;

    @Resource
    private SessionContext sessionContext;

    @Nonnull
    public Member getAuthenticatedUser()
    {
        final String principalName = sessionContext.getCallerPrincipal().getName();
        final Member member = memberDAO.findByEmail(principalName);
        if (null == member) {
            throw new IllegalStateException("Cannot find member by principal name: " + principalName);
        }
        return member;
    }

    @Nonnull
    public Member getByEmail(@Nonnull String email)
    {
        final Member member = memberDAO.findByEmail(email);
        if (null == member) {
            throw new NoResultException("No member with email '" + email + "' found");
        }
        return member;
    }

    @PermitAll
    @Nonnull
    public Member register(Member member) throws EmailAleadyRegisteredException
    {
        final String email = member.getEmail();
        Member user = memberDAO.findByEmail(email);
        if (null != user) {
            throw new EmailAleadyRegisteredException("Email already registered: " + email);
        }

        return memberDAO.save(member);
    }
}
