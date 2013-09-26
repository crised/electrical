package cl.telematic.business;

import cl.telematic.dao.StatsDAO;
import cl.telematic.model.DemandStats;
import cl.telematic.model.Device;
import cl.telematic.model.EnergyStats;
import cl.telematic.rest.domain.InstantStats;
import cl.telematic.rest.domain.DeviceStats;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.inject.Inject;
import javax.validation.Validator;
import java.util.Calendar;
import java.util.Date;

@Stateless
public class StatsManager {

    @EJB
    private DeviceManager deviceManager;

    @EJB
    private StatsDAO statsDAO;

    @EJB
    private UserManager userManager;

    @Inject
    private Validator validator;

    @Nullable
    public DeviceStats getDeviceStats(@Nonnull Long id, @Nonnull Date start, @Nonnull Date end)
    {
        final Device device = deviceManager.getDevice(id);
        Calendar calendar = toDayPrecision(start);
        start = calendar.getTime();
        calendar = toDayPrecision(end);
        calendar.add(Calendar.DATE, 1);
        calendar.add(Calendar.MILLISECOND, -1);
        end = calendar.getTime();
        return statsDAO.getStats(device, start, end);
    }

    @Nonnull
    public InstantStats save(@Nonnull Long deviceId, @Nonnull InstantStats stats)
    {
//        Check if device exists
        deviceManager.getDevice(deviceId);
        return statsDAO.save(stats);
    }

    @Nonnull
    public EnergyStats save(@Nonnull Long deviceId, @Nonnull EnergyStats stats)
    {
        Device device = deviceManager.getDevice(deviceId);
        stats.setDevice(device);
        return statsDAO.save(stats);
    }

    @Nonnull
    public DemandStats save(@Nonnull Long deviceId, @Nonnull DemandStats stats)
    {
        Device device = deviceManager.getDevice(deviceId);
        stats.setDevice(device);
        return statsDAO.save(stats);
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
