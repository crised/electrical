package cl.telematic.rest;

import cl.telematic.business.StatsManager;
import cl.telematic.model.ActiveTotalEnergyStats;
import cl.telematic.model.ElectricalStats;

import javax.annotation.Nonnull;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.ws.rs.core.Response;

@Stateless
public class StatsResourceImpl implements StatsResource {

    @EJB
    private StatsManager statsManager;

    @Override
    public Response save(@Nonnull Long deviceId, @Nonnull ActiveTotalEnergyStats stats)
    {
        statsManager.save(deviceId, stats);
        return Response.ok().build();
    }

    @Override
    public Response save(@Nonnull Long deviceId, @Nonnull ElectricalStats stats)
    {
        statsManager.save(deviceId, stats);
        return Response.ok().build();
    }
}
