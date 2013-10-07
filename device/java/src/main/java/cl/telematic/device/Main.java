package cl.telematic.device;

import cl.telematic.rest.StatsResource;
import org.jboss.resteasy.client.jaxrs.ResteasyClientBuilder;
import org.jboss.resteasy.client.jaxrs.ResteasyWebTarget;

import javax.ws.rs.client.Client;
import javax.ws.rs.core.Response;
import java.net.URI;
import java.net.URISyntaxException;
import java.sql.SQLException;
import java.util.Iterator;
import java.util.List;

public class Main {

    private static void handleResponse(String stasName, Response response, Iterator<?> iterator)
    {
        if (Response.Status.OK.getStatusCode() != response.getStatus()) {
            System.err.println("Cannot send " + stasName + " stats. Response code:" + response.getStatusInfo());
            if (response.hasEntity()) {
                System.err.println(response.readEntity(String.class));
            }
            iterator.remove();
        }
        response.close();
    }

    public static void main(String[] args) throws URISyntaxException
    {
        if (args.length < 2) {
            System.err.println("Usage:");
            System.err.println("\tjava -cp ... -jar " + Main.class.getCanonicalName() + " DEVICE_ID REST_API_URL");
            System.err.println("\n\ti.e. java -cp ... -jar " + Main.class.getCanonicalName() + "http://23.21.60.18:8080/electricmeter/rest");
        }
        final Long deviceId = Long.parseLong(args[0]);
        final URI restApiBaseURL = new URI(args[1]);
        Client client = ResteasyClientBuilder.newClient();
        client.register(AuthFilter.class);
        ResteasyWebTarget webTarget = (ResteasyWebTarget) client.target(restApiBaseURL);
        final StatsResource statsResource = webTarget.proxy(StatsResource.class);

        final Database database = new Database();

        //noinspection InfiniteLoopStatement
        do {
            System.out.println("Checking db...");
            try {
                System.out.println("Entering demand...");

                sendDemandStats(database, statsResource, deviceId);

            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
            try {
                System.out.println("Entering Energy...");

                sendEnergyStats(database, statsResource, deviceId);
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
            try {
                System.out.println("Entering Instant...");

                sendInstantStats(database, statsResource, deviceId);
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
            System.out.println("...done. Sleeping.");
//                    Wait for 1 minute
            try {
                Thread.sleep(1000 * 60);
            } catch (InterruptedException ignore) {
//                        We've been interrupted, no problem
            }
        } while (true);
    }

    private static void sendDemandStats(Database database, StatsResource statsResource, Long deviceId) throws SQLException
    {
        List<LocalDemandStats> statses = database.getDemandStats();
        for (Iterator<LocalDemandStats> iterator = statses.iterator(); iterator.hasNext(); ) {
            LocalDemandStats stats = iterator.next();
            final Response response = statsResource.save(deviceId, stats);
            handleResponse("demand", response, iterator);
        }
        System.out.println("Sent " + statses.size() + " demand records");
        database.removeDemandStats(statses);
    }

    private static void sendEnergyStats(Database database, StatsResource statsResource, Long deviceId) throws SQLException
    {
        List<LocalEnergyStats> statses = database.getEnergyStats();
        for (Iterator<LocalEnergyStats> iterator = statses.iterator(); iterator.hasNext(); ) {
            LocalEnergyStats stats = iterator.next();
            final Response response = statsResource.save(deviceId, stats);
            handleResponse("energy", response, iterator);
        }
        System.out.println("Sent " + statses.size() + " energy records");
        database.removeEnergyStats(statses);
    }

    private static void sendInstantStats(Database database, StatsResource statsResource, Long deviceId) throws SQLException
    {
        List<LocalInstantStats> statses = database.getInstantStats();
        for (Iterator<LocalInstantStats> iterator = statses.iterator(); iterator.hasNext(); ) {
            LocalInstantStats stats = iterator.next();
            final Response response = statsResource.save(deviceId, stats);
            handleResponse("instant", response, iterator);
        }
        System.out.println("Sent " + statses.size() + " instant records");
        database.removeInstantStats(statses);
    }
}
