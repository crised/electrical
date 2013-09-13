package cl.telematic.rest;

import cl.telematic.model.ActiveTotalEnergyStats;
import cl.telematic.model.ElectricalStats;

import javax.validation.Valid;
import javax.validation.constraints.NotNull;
import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Consumes({MediaType.APPLICATION_JSON})
@Produces({MediaType.APPLICATION_JSON})
@Path("/stats")
public interface StatsResource {

    /**
     * There is a bug RESTEASY-913 that does not allow @NotNull @Valid annotation to param
     * TODO uncomment @ValidateRequest once RESTEASY-913 is fixed
     */
//    @ValidateRequest
    @POST
    @Path("/{device}/ELE")
    Response save(@NotNull @PathParam("device") Long deviceId, @NotNull @Valid ElectricalStats stats);

    /**
     * There is a bug RESTEASY-913 that does not allow @NotNull @Valid annotation to param
     * TODO uncomment @ValidateRequest once RESTEASY-913 is fixed
     */
    //    @ValidateRequest
    @POST
    @Path("/{device}/ATE")
    Response save(@NotNull @PathParam("device") Long deviceId, @NotNull @Valid ActiveTotalEnergyStats stats);
}
