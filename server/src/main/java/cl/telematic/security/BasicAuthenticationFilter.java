package cl.telematic.security;

import org.jboss.logging.Logger;
import org.jboss.resteasy.util.Base64;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.container.ContainerRequestContext;
import javax.ws.rs.container.ContainerRequestFilter;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MultivaluedMap;
import javax.ws.rs.core.Response;
import javax.ws.rs.ext.Provider;
import java.io.IOException;
import java.util.StringTokenizer;

@Provider
@Authenticated
public class BasicAuthenticationFilter implements ContainerRequestFilter {

    private static final String AUTHENTICATION_SCHEME = "Basic";

    private static final String AUTHORIZATION_PROPERTY = "Authorization";

    private static Logger LOGGER = Logger.getLogger(BasicAuthenticationFilter.class);

    @Context
    private HttpServletRequest httpServletRequest;

    @Override
    public void filter(ContainerRequestContext requestContext) throws IOException
    {
        if (null != requestContext.getSecurityContext().getUserPrincipal()) {
            return;
        }
        //Get request headers
        final MultivaluedMap<String, String> headers = requestContext.getHeaders();

        //Fetch authorization header
        final String authorization = headers.getFirst(AUTHORIZATION_PROPERTY);

        //If no authorization information present; block access
        if (authorization == null || authorization.isEmpty()) {
            sendUnauthorized(requestContext);
            return;
        }

        //Get encoded username and password
        final String encodedUserPassword = authorization.replaceFirst(AUTHENTICATION_SCHEME + " ", "");

        //Decode username and password
        String usernameAndPassword;
        try {
            usernameAndPassword = new String(Base64.decode(encodedUserPassword));
        } catch (IOException e) {
            LOGGER.error("Cannot decode credentials Base64", e);
            requestContext.abortWith(Response.serverError().build());
            return;
        }

        //Split username and password tokens
        final StringTokenizer tokenizer = new StringTokenizer(usernameAndPassword, ":");
        final String username = tokenizer.hasMoreTokens() ? tokenizer.nextToken() : "";
        final String password = tokenizer.hasMoreTokens() ? tokenizer.nextToken() : "";
        try {
            httpServletRequest.login(username, password);
        } catch (ServletException e) {
            LOGGER.error("Cannot login", e);
            sendUnauthorized(requestContext);
        }
    }

    private void sendUnauthorized(ContainerRequestContext requestContext)
    {
        requestContext.abortWith(Response.status(Response.Status.UNAUTHORIZED).header("WWW-Authenticate", "Basic realm=\"Realm\"").build());
    }
}
