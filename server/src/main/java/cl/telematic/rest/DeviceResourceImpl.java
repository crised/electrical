package cl.telematic.rest;

import cl.telematic.business.DeviceManager;

import javax.annotation.Nonnull;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.Response;

@Stateless
public class DeviceResourceImpl implements DeviceResource {

    @EJB
    private DeviceManager deviceManager;

    @Context
    private HttpServletRequest request;

    @Override
    public Response ping(@Nonnull Long id)
    {
        deviceManager.ping(id, request.getRemoteAddr());
        return Response.ok().build();
    }
}
