package cl.telematic.dao;

import cl.telematic.model.DemandStats;
import cl.telematic.model.Device;
import cl.telematic.model.EnergyStats;
import cl.telematic.rest.domain.DeviceStats;
import cl.telematic.rest.domain.InstantStats;
import org.jboss.logging.Logger;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.NoResultException;
import javax.persistence.PersistenceContext;
import java.util.Date;
import java.util.List;

@Stateless
public class StatsDAO {

    private static final Logger LOGGER = Logger.getLogger(StatsDAO.class);

    /**
     * Alternatively we can user Infinispan:
     * <p/>
     * &amp;Resource(lookup="java:jboss/infinispan/my-container-name")
     * private org.infinispan.manager.CacheContainer container;
     */
    @EJB
    private CacheManager cacheManager;

    @PersistenceContext
    private EntityManager entityManager;

    @Nullable
    public DeviceStats getStats(@Nonnull Device device, @Nonnull Date start, @Nonnull Date end)
    {
        Long activePowerTotalMax18_23;
        try {
            activePowerTotalMax18_23 = (Long) entityManager.createQuery(
                "select max(kva_block_demand) from DemandStats where device.id=:id and createdOn between :start and :end and hour between 18 and 23 group by device.id")
                .setParameter("id", device.getId())
                .setParameter("start", start)
                .setParameter("end", end)
                .getSingleResult();
        } catch (NoResultException e) {
            activePowerTotalMax18_23 = null;
        }

        final EnergyStats energyStatsA = findOldestEnergyStatsLessThan(start);
        final EnergyStats energyStatsB = findOldestEnergyStatsLessThan(end);

        final String query = "select " + "avg(kw_import_block_demand),max(kw_import_block_demand),min(kw_import_block_demand),"
            + "avg(kva_block_demand),max(kva_block_demand),min(kva_block_demand),"
            + "avg(kvar_import_block_demand),max(kvar_import_block_demand),min(kvar_import_block_demand),"
            + " device.id from DemandStats where device.id=:id and createdOn between :start and :end group by device.id, device.name";
        Object[] result;
        try {
            result = (Object[]) entityManager.createQuery(query)
                .setParameter("id", device.getId())
                .setParameter("start", start)
                .setParameter("end", end)
                .getSingleResult();
        } catch (NoResultException e) {
            result = null;
        }
        Date lastUpdate;
        try {
            lastUpdate = entityManager.createQuery("select max(createdOn) from StatsEntry where device.id=:id", Date.class)
                .setParameter("id", device.getId())
                .getSingleResult();
        } catch (NoResultException e) {
            lastUpdate = null;
        }
        return toDeviceStats(device, result, energyStatsA, energyStatsB, activePowerTotalMax18_23, lastUpdate);
    }

    @Nullable
    public InstantStats findInstantStats(@Nonnull Long deviceId)
    {
        return cacheManager.getInstantStats(deviceId);
    }

    @Nonnull
    public InstantStats save(@Nonnull Long deviceId, @Nonnull InstantStats stats)
    {
        cacheManager.putInstantStats(deviceId, stats);
        return stats;
    }

    @Nonnull
    public EnergyStats save(@Nonnull EnergyStats stats)
    {
        return entityManager.merge(stats);
    }

    @Nonnull
    public DemandStats save(@Nonnull DemandStats stats)
    {
        return entityManager.merge(stats);
    }

    @Nullable
    private EnergyStats findOldestEnergyStatsLessThan(@Nonnull Date date)
    {
        final List<EnergyStats> resultList = entityManager.createQuery(
            "from EnergyStats where createdOn in (select max(createdOn) from EnergyStats where createdOn<=:date)", EnergyStats.class)
            .setParameter("date", date)
            .getResultList();
        if (resultList.size() > 1) {
            LOGGER.warnv("There is more than one EnergyStats row for date {0}", date);
        }
        return resultList.isEmpty() ? null : resultList.get(0);
    }

    @Nonnull
    private DeviceStats.Aggregate toAggregate(@Nonnull Object[] result, int offset)
    {
        return new DeviceStats.Aggregate((Number) result[offset], (Number) result[offset + 1], (Number) result[offset + 2]);
    }

    @Nonnull
    private DeviceStats toDeviceStats(@Nonnull Device device, @Nullable Object[] result, EnergyStats energyStatsA, EnergyStats energyStatsB,
                                      @Nullable Long activePowerTotalMax18_23, @Nullable Date lastUpdate)
    {
        final DeviceStats deviceStats = new DeviceStats();
        if (null != result) {
            int i = 0;
            deviceStats.setKw_import_block_demand(toAggregate(result, i));
            deviceStats.setKva_block_demand(toAggregate(result, i));
            deviceStats.setKvar_import_block_demand(toAggregate(result, i));
        }
        if (null == energyStatsA) {
            if (null != energyStatsB) {
                deviceStats.setKwh_import(energyStatsB.getKwh_import());
                deviceStats.setKwh_import_l1(energyStatsB.getKwh_import_l1());
                deviceStats.setKwh_import_l2(energyStatsB.getKwh_import_l2());
                deviceStats.setKwh_import_l3(energyStatsB.getKwh_import_l3());
            }
        } else if (null != energyStatsB) {
            deviceStats.setKwh_import(energyStatsB.getKwh_import() - energyStatsA.getKwh_import());
            deviceStats.setKwh_import_l1(energyStatsB.getKwh_import_l1() - energyStatsA.getKwh_import_l1());
            deviceStats.setKwh_import_l2(energyStatsB.getKwh_import_l2() - energyStatsA.getKwh_import_l1());
            deviceStats.setKwh_import_l3(energyStatsB.getKwh_import_l3() - energyStatsA.getKwh_import_l1());
        }
        deviceStats.setDeviceId(device.getId());
        deviceStats.setDeviceName(device.getName());
        deviceStats.setActivePowerTotalMax18_23(activePowerTotalMax18_23);

        final InstantStats instantStats = cacheManager.getInstantStats(device.getId());

        if (null != instantStats) {
            deviceStats.setTotal_kw(instantStats.getTotal_kw());
            deviceStats.setTotal_pf(instantStats.getTotal_pf());
            deviceStats.setV1_voltage(instantStats.getV1_voltage());
            deviceStats.setV2_voltage(instantStats.getV2_voltage());
            deviceStats.setV3_voltage(instantStats.getV3_voltage());
            if (lastUpdate != null && instantStats.getCreatedOn() != null && lastUpdate.before(instantStats.getCreatedOn())) {
                deviceStats.setLastUpdate(instantStats.getCreatedOn());
            }
        }
        if (null == deviceStats.getLastUpdate()) {
            deviceStats.setLastUpdate(lastUpdate);
        }
        return deviceStats;
    }
}
