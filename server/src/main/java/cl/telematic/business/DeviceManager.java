package cl.telematic.business;

import cl.telematic.dao.DeviceDAO;
import cl.telematic.model.Device;
import cl.telematic.model.DeviceStats;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import java.util.Calendar;
import java.util.Date;
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

    @Nullable
    public DeviceStats getDeviceStats(@Nonnull Long id, @Nonnull Date start, @Nonnull Date end)
    {
        final Device device = getDevice(id);
        Calendar calendar = toDayPrecision(start);
        start = calendar.getTime();
        calendar = toDayPrecision(end);
        calendar.add(Calendar.DATE, 1);
        calendar.add(Calendar.MILLISECOND, -1);
        end = calendar.getTime();
        return deviceDAO.getStats(id, start, end);
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

    private Calendar toDayPrecision(Date start)
    {
        final Calendar calendar = Calendar.getInstance();
        calendar.setTime(start);
        calendar.set(Calendar.HOUR_OF_DAY, 0);
        calendar.set(Calendar.MINUTE, 0);
        calendar.set(Calendar.SECOND, 0);
        calendar.set(Calendar.MILLISECOND, 0);
        return calendar;
    }
}
