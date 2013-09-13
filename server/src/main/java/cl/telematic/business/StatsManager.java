package cl.telematic.business;

import cl.telematic.dao.StatsDAO;
import cl.telematic.model.ActiveTotalEnergyStats;
import cl.telematic.model.Device;
import cl.telematic.model.ElectricalStats;

import javax.annotation.Nonnull;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.inject.Inject;
import javax.validation.Validator;

@Stateless
public class StatsManager {

    @EJB
    private DeviceManager deviceManager;

    @EJB
    private MemberManager memberManager;

    @EJB
    private StatsDAO statsDAO;

    @Inject
    private Validator validator;

    @Nonnull
    public ActiveTotalEnergyStats save(@Nonnull Long deviceId, @Nonnull ActiveTotalEnergyStats stats)
    {
        Device device = deviceManager.getDevice(deviceId);
        stats.setDevice(device);
        return statsDAO.save(stats);
    }

    @Nonnull
    public ElectricalStats save(@Nonnull Long deviceId, @Nonnull ElectricalStats stats)
    {
        Device device = deviceManager.getDevice(deviceId);
        stats.setDevice(device);
        return statsDAO.save(stats);
    }
}
