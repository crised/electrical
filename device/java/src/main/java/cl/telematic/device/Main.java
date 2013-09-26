package cl.telematic.device;

import cl.telematic.rest.StatsResource;
import org.jboss.resteasy.client.jaxrs.ResteasyClientBuilder;
import org.jboss.resteasy.client.jaxrs.ResteasyWebTarget;

import javax.ws.rs.client.Client;
import java.sql.SQLException;
import java.util.List;

public class Main {

    public static void main(String[] args)
    {
        if (args.length < 2) {
            System.err.println("Usage:");
            System.err.println("\tjava -cp ... -jar " + Main.class.getCanonicalName() + " DEVICE_ID REST_API_URL");
            System.err.println("\n\ti.e. java -cp ... -jar " + Main.class.getCanonicalName() + " 1 http://localhost:8080/electricmeter/rest");
        }
        final Long deviceId = Long.parseLong(args[0]);
        final String restApiBaseURL = args[1];
        Client client = ResteasyClientBuilder.newClient();
        ResteasyWebTarget webTarget = (ResteasyWebTarget) client.target(restApiBaseURL);
        final StatsResource statsResource = webTarget.proxy(StatsResource.class);

        final Database database = new Database();
        final Thread thread = new Thread(new Runnable() {
            @Override
            public void run()
            {
                //noinspection InfiniteLoopStatement
                do {
                    System.out.println("Checking db...");
                    try {
                        sendDemandStats(database, statsResource, deviceId);
                    } catch (SQLException e) {
                        throw new RuntimeException(e);
                    }
                    try {
                        sendEnergyStats(database, statsResource, deviceId);
                    } catch (SQLException e) {
                        throw new RuntimeException(e);
                    }
                    try {
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
        });
        thread.start();
    }

    private static void sendDemandStats(Database database, StatsResource statsResource, Long deviceId) throws SQLException
    {
        List<LocalDemandStats> statses = database.getDemandStats();
        for (LocalDemandStats stats : statses) {
            statsResource.save(deviceId, stats).close();
        }
        System.out.println("Sent " + statses.size() + " demand records");
        database.removeDemandStats(statses);
    }

    private static void sendEnergyStats(Database database, StatsResource statsResource, Long deviceId) throws SQLException
    {
        List<LocalEnergyStats> statses = database.getEnergyStats();
        for (LocalEnergyStats stats : statses) {
            statsResource.save(deviceId, stats).close();
        }
        System.out.println("Sent " + statses.size() + " energy records");
        database.removeEnergyStats(statses);
    }

    private static void sendInstantStats(Database database, StatsResource statsResource, Long deviceId) throws SQLException
    {
        List<LocalInstantStats> statses = database.getInstantStats();
        for (LocalInstantStats stats : statses) {
            statsResource.save(deviceId, stats).close();
        }
        System.out.println("Sent " + statses.size() + " instant records");
        database.removeInstantStats(statses);
    }
}
