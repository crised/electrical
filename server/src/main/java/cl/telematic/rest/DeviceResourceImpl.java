package cl.telematic.rest;

import cl.telematic.business.DeviceManager;
import cl.telematic.model.Device;
import cl.telematic.model.DeviceStats;
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

    //TODO return objects from rest domain (hide user password)
    @EJB
    private DeviceManager deviceManager;

    @Context
    private HttpServletRequest request;

    @Authenticated
    @Override
    public DeviceStats getDeviceStats(@Nonnull Long id, @Nonnull Long start, @Nonnull Long end)
    {
        return deviceManager.getDeviceStats(id, new Date(start), new Date(end));
    }

    @Authenticated
    @Override
    public List<Device> getMyDevices()
    {
        return deviceManager.getMyDevices();
    }

    @Override
    public Response ping(@Nonnull Long id)
    {
        deviceManager.ping(id, request.getRemoteAddr());
        return Response.ok().build();
    }
}
