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

    public void ping(@Nonnull Long id, @Nonnull String remoteAddr)
    {
        final Device device = deviceDAO.get(id);
        device.setIp(remoteAddr);
        deviceDAO.save(device);
    }
}
