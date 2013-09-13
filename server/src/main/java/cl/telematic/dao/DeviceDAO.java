package cl.telematic.dao;

import cl.telematic.model.Device;

import javax.annotation.Nonnull;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.NoResultException;
import javax.persistence.PersistenceContext;

@Stateless
public class DeviceDAO {

    @PersistenceContext
    private EntityManager entityManager;

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
