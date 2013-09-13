package cl.telematic.business;

import cl.telematic.dao.DeviceDAO;
import cl.telematic.model.Device;

import javax.annotation.Nonnull;
import javax.ejb.EJB;
import javax.ejb.Stateless;

@Stateless
public class DeviceManager {

    @EJB
    private DeviceDAO deviceDAO;

    @Nonnull
    public Device getDevice(@Nonnull Long deviceId)
    {
        return deviceDAO.get(deviceId);
    }
}
