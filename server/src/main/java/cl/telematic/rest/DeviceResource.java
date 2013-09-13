package cl.telematic.rest;

import javax.validation.constraints.NotNull;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.core.Response;

@Path("/device")
public interface DeviceResource {

    @POST
    @Path("/{device}/ping")
    Response ping(@NotNull @PathParam("device") Long id);
}
