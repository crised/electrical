package cl.telematic.device;

import org.jboss.resteasy.util.Base64;

import javax.ws.rs.client.ClientRequestContext;
import javax.ws.rs.client.ClientRequestFilter;
import java.io.IOException;

public class AuthFilter implements ClientRequestFilter {

    @Override
    public void filter(ClientRequestContext requestContext) throws IOException
    {
        final String username = "device"; //device
        final String password = "ecived"; //ecived
        String encodedCredentials = Base64.encodeBytes((username + ":" + password).getBytes());
        requestContext.getHeaders().putSingle("Authorization", "Basic " + encodedCredentials);
    }
}
