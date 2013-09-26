package cl.telematic.business;

import cl.telematic.dao.DeviceDAO;
import cl.telematic.model.Device;

import javax.annotation.Nonnull;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import java.util.List;

@Stateless
public class DeviceManager {

    @EJB
    private DeviceDAO deviceDAO;

    @EJB
    private UserManager userManager;

    @Nonnull
    public Device getDevice(@Nonnull Long deviceId)
    {
//        TODO we need to check access here
        return deviceDAO.get(deviceId);
    }

    public List<Device> getMyDevices()
    {
        return deviceDAO.getDevicesByUser(userManager.getAuthenticatedUser().getId());
    }

    public void ping(@Nonnull Long id, @Nonnull String remoteAddr)
    {
        final Device device = deviceDAO.get(id);
        device.setIp(remoteAddr);
        deviceDAO.save(device);
    }
}
