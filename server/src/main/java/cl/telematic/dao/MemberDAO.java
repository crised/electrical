package cl.telematic.dao;

import cl.telematic.model.Member;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.NoResultException;
import javax.persistence.PersistenceContext;
import java.util.List;

@Stateless
public class MemberDAO {

    @PersistenceContext
    private EntityManager entityManager;

    @Nonnull
    public List<Member> findAllOrderedByName()
    {
        return entityManager.createQuery("from Member order by name", Member.class).getResultList();
    }

    public Member findByEmail(@Nonnull String email) throws NoResultException
    {
        try {
            return entityManager.createQuery("from Member where email=:email", Member.class).setParameter("email", email).getSingleResult();
        } catch (NoResultException e) {
            return null;
        }
    }

    @Nullable
    public Member findById(long id)
    {
        return entityManager.find(Member.class, id);
    }

    @Nonnull
    public Member get(@Nonnull Long id)
    {
        final Member member = entityManager.find(Member.class, id);
        if (null == member) {
            throw new NoResultException("Member #" + id + " not found");
        }
        return member;
    }

    @Nonnull
    public Member save(@Nonnull Member member)
    {
        entityManager.persist(member);
        return member;
    }
}
