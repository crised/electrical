package cl.telematic.rest;

import cl.telematic.rest.domain.User;

import javax.validation.constraints.NotNull;
import javax.ws.rs.Consumes;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import java.util.List;

@Consumes(MediaType.APPLICATION_JSON)
@Produces(MediaType.APPLICATION_JSON)
@Path("/user")
public interface UserResource {

    @GET
    @Path("/")
    List<User> getAllUsers();

    @GET
    @Path("/me")
    User getAuthenticatedUser();

    @GET
    @Path("/{id:[0-9][0-9]*}")
    User getUser(@NotNull @PathParam("id") Long id);

    @Consumes({MediaType.APPLICATION_FORM_URLENCODED})
    @POST
    @Path("/me/password")
    User changePassword(@NotNull @FormParam("current") String currentPassword, @NotNull @FormParam("new") String newPassword);

    @POST
    @Path("/exists/{email}")
    boolean isEmailRegistered(@NotNull @PathParam("email") String email);

    @POST
    @Path("/")
    User register(@NotNull User user);

    @POST
    @Path("/{id}")
    User update(@NotNull @PathParam("id") Long id, @NotNull User user);
}
