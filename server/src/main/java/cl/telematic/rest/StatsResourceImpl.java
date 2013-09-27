package cl.telematic.rest;

import cl.telematic.business.StatsManager;
import cl.telematic.rest.domain.DemandStats;
import cl.telematic.rest.domain.EnergyStats;
import cl.telematic.rest.domain.InstantStats;
import cl.telematic.security.Authenticated;

import javax.annotation.Nonnull;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.ws.rs.core.Response;

@Stateless
public class StatsResourceImpl implements StatsResource {

    @EJB
    private DomainConverter domainConverter;

    @EJB
    private StatsManager statsManager;

    @Authenticated
    @Override
    public Response save(@Nonnull Long deviceId, @Nonnull InstantStats stats)
    {
        statsManager.save(deviceId, stats);
        return Response.ok().build();
    }

    @Authenticated
    @Override
    public Response save(@Nonnull Long deviceId, @Nonnull DemandStats stats)
    {
        statsManager.save(deviceId, domainConverter.toDBDomain(stats));
        return Response.ok().build();
    }

    @Authenticated
    @Override
    public Response save(@Nonnull Long deviceId, @Nonnull EnergyStats stats)
    {
        statsManager.save(deviceId, domainConverter.toDBDomain(stats));
        return Response.ok().build();
    }
}
