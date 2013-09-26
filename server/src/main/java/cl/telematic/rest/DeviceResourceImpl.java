package cl.telematic.rest;

import cl.telematic.business.DeviceManager;
import cl.telematic.business.StatsManager;
import cl.telematic.rest.domain.Device;
import cl.telematic.rest.domain.DeviceStats;
import cl.telematic.security.Authenticated;

import javax.annotation.Nonnull;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.Response;
import java.util.Date;
import java.util.List;

@Stateless
public class DeviceResourceImpl implements DeviceResource {

    @EJB
    private DeviceManager deviceManager;

    @EJB
    private DomainConverter domainConverter;

    @Context
    private HttpServletRequest request;

    @EJB
    private StatsManager statsManager;

    @Authenticated
    @Override
    public DeviceStats getDeviceStats(@Nonnull Long id, @Nonnull Long start, @Nonnull Long end)
    {
        return statsManager.getDeviceStats(id, new Date(start), new Date(end));
    }

    @Authenticated
    @Override
    public List<Device> getMyDevices()
    {
        return domainConverter.toRestDeviceList(deviceManager.getMyDevices());
    }

    @Override
    public Response ping(@Nonnull Long id)
    {
        deviceManager.ping(id, request.getRemoteAddr());
        return Response.ok().build();
    }
}
