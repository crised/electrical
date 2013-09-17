package cl.telematic.dao;

import cl.telematic.model.Device;
import cl.telematic.model.DeviceStats;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.NoResultException;
import javax.persistence.PersistenceContext;
import java.util.Date;
import java.util.List;

@Stateless
public class DeviceDAO {

    @PersistenceContext
    private EntityManager entityManager;

    @Nonnull
    public List<Device> getDevicesByMember(@Nonnull Long memberId)
    {
        return entityManager.createQuery("select d from Device d where d.member.id=:memberId", Device.class).setParameter("memberId", memberId).getResultList();
    }

    @Nullable
    public DeviceStats getStats(@Nonnull Long id, @Nonnull Date start, @Nonnull Date end)
    {
        Number activePowerTotalMax18_23;
        try {
            activePowerTotalMax18_23 = (Number) entityManager.createQuery(
                "select max(activePowerTotal) from ElectricalStats where device.id=:id and createdOn between :start and :end and hour between 18 and 23 group by device.id")
                .setParameter("id", id)
                .setParameter("start", start)
                .setParameter("end", end)
                .getSingleResult();
        } catch (NoResultException e) {
            activePowerTotalMax18_23 = null;
        }
        final String query =
            "select " + "avg(activePowerTotal),max(activePowerTotal),min(activePowerTotal)," + "avg(activePower1),max(activePower1),min(activePower1),"
                + "avg(activePower2),max(activePower2),min(activePower2)," + "avg(activePower3),max(activePower3),min(activePower3),"

                + "avg(apparentPowerTotal),max(apparentPowerTotal),min(apparentPowerTotal)," + "avg(apparentPower1),max(apparentPower1),min(apparentPower1),"
                + "avg(apparentPower2),max(apparentPower2),min(apparentPower2)," + "avg(apparentPower3),max(apparentPower3),min(apparentPower3),"

                + "avg(current1),max(current1),min(current1)," + "avg(current2),max(current2),min(current2)," + "avg(current3),max(current3),min(current3),"

                + "avg(frequency1),max(frequency1),min(frequency1)," + "avg(frequency2),max(frequency2),min(frequency2),"
                + "avg(frequency3),max(frequency3),min(frequency3),"

                + "avg(reactivePowerTotal),max(reactivePowerTotal),min(reactivePowerTotal)," + "avg(reactivePower1),max(reactivePower1),min(reactivePower1),"
                + "avg(reactivePower2),max(reactivePower2),min(reactivePower2)," + "avg(reactivePower3),max(reactivePower3),min(reactivePower3),"

                + "avg(voltage1),max(voltage1),min(voltage1)," + "avg(voltage2),max(voltage2),min(voltage2)," + "avg(voltage3),max(voltage3),min(voltage3),"

                + " device.id, device.name from ElectricalStats where device.id=:id and createdOn between :start and :end group by device.id, device.name";
        try {
            final Object[] result = (Object[]) entityManager.createQuery(query)
                .setParameter("id", id)
                .setParameter("start", start)
                .setParameter("end", end)
                .getSingleResult();
            return toDeviceStats(result, activePowerTotalMax18_23);
        } catch (NoResultException e) {
            return null;
        }
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

    @Nonnull
    private DeviceStats.Aggregate toAggregate(@Nonnull Object[] result, int offset)
    {
        return new DeviceStats.Aggregate((Number) result[offset], (Number) result[offset + 1], (Number) result[offset + 2]);
    }

    @Nonnull
    private DeviceStats toDeviceStats(@Nonnull Object[] result, @Nullable Number activePowerTotalMax18_23)
    {
        final DeviceStats deviceStats = new DeviceStats();
        int i = 0;
        deviceStats.activePowerTotal = toAggregate(result, i);
        deviceStats.activePower1 = toAggregate(result, i += 3);
        deviceStats.activePower2 = toAggregate(result, i += 3);
        deviceStats.activePower3 = toAggregate(result, i += 3);
        deviceStats.apparentPowerTotal = toAggregate(result, i += 3);
        deviceStats.apparentPower1 = toAggregate(result, i += 3);
        deviceStats.apparentPower2 = toAggregate(result, i += 3);
        deviceStats.apparentPower3 = toAggregate(result, i += 3);
        deviceStats.current1 = toAggregate(result, i += 3);
        deviceStats.current2 = toAggregate(result, i += 3);
        deviceStats.current3 = toAggregate(result, i += 3);
        deviceStats.frequency1 = toAggregate(result, i += 3);
        deviceStats.frequency2 = toAggregate(result, i += 3);
        deviceStats.frequency3 = toAggregate(result, i += 3);
        deviceStats.reactivePower1 = toAggregate(result, i += 3);
        deviceStats.reactivePower2 = toAggregate(result, i += 3);
        deviceStats.reactivePower3 = toAggregate(result, i += 3);
        deviceStats.reactivePowerTotal = toAggregate(result, i += 3);
        deviceStats.voltage1 = toAggregate(result, i += 3);
        deviceStats.voltage2 = toAggregate(result, i += 3);
        deviceStats.voltage3 = toAggregate(result, i += 3);
        deviceStats.deviceId = (Long) result[i += 3];
        deviceStats.deviceName = (String) result[i += 1];
        deviceStats.activePowerTotalMax18_23 = null == activePowerTotalMax18_23 ? null : activePowerTotalMax18_23.doubleValue();
        return deviceStats;
    }
}
