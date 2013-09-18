package cl.telematic.dao;

import cl.telematic.model.User;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.NoResultException;
import javax.persistence.PersistenceContext;
import java.util.List;

@Stateless
public class UserDAO {

    @PersistenceContext
    private EntityManager entityManager;

    @Nonnull
    public List<User> getAllUsersOrderedByName()
    {
        return entityManager.createQuery("from User order by name", User.class).getResultList();
    }

    @Nullable
    public User findByEmail(@Nonnull String email) throws NoResultException
    {
        try {
            return entityManager.createQuery("from User where email=:email", User.class).setParameter("email", email).getSingleResult();
        } catch (NoResultException e) {
            return null;
        }
    }

    @Nonnull
    public User get(@Nonnull Long id)
    {
        final User user = entityManager.find(User.class, id);
        if (null == user) {
            throw new NoResultException("User #" + id + " not found");
        }
        return user;
    }

    @Nonnull
    public User save(@Nonnull User user)
    {
        entityManager.persist(user);
        return user;
    }
}
