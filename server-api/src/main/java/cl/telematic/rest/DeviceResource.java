package cl.telematic.rest;

import cl.telematic.rest.domain.Device;
import cl.telematic.rest.domain.DeviceStats;

import javax.validation.constraints.NotNull;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import java.util.List;

@Consumes({MediaType.APPLICATION_JSON})
@Produces({MediaType.APPLICATION_JSON})
@Path("/device")
public interface DeviceResource {

    @GET
    @Path("/{device}/stats/{start}/{end}")
    DeviceStats getDeviceStats(@NotNull @PathParam("device") Long id, @NotNull @PathParam("start") Long start, @NotNull @PathParam("end") Long end);

    @GET
    @Path("/")
    List<Device> getMyDevices();

    @POST
    @Path("/{device}/ping")
    Response ping(@NotNull @PathParam("device") Long id);
}
