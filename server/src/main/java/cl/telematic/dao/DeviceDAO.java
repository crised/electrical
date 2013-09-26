package cl.telematic.dao;

import cl.telematic.model.Device;

import javax.annotation.Nonnull;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.NoResultException;
import javax.persistence.PersistenceContext;
import java.util.List;

@Stateless
public class DeviceDAO {

    @PersistenceContext
    private EntityManager entityManager;

    @Nonnull
    public List<Device> getDevicesByUser(@Nonnull Long userId)
    {
        return entityManager.createQuery("select d from Device d where d.user.id=:userId", Device.class).setParameter("userId", userId).getResultList();
    }

    @Nonnull
    public Device get(@Nonnull Long id)
    {
        final Device device = entityManager.find(Device.class, id);
        if (null == device) {
            throw new NoResultException("Device #" + id + " not found");
        }
        return device;
    }

    @Nonnull
    public Device save(@Nonnull Device device)
    {
        return entityManager.merge(device);
    }
}
