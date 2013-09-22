import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.URL;
import java.net.HttpURLConnection;

public class Net {

    private String url = "http://requestb.in/o0cy4ko0";
    private String USER_AGENT = "Mozilla/5.0";


    void sendPost(byte[] bytes) throws Exception {

        URL u = new URL(url);
        HttpURLConnection con = (HttpURLConnection) u.openConnection();

        //add reuqest header
        con.setRequestMethod("POST");
        con.setRequestProperty("User-Agent", USER_AGENT);
        con.setRequestProperty("Accept-Language", "en-US,en;q=0.5");
        con.setRequestProperty("Content-Type", "application/json");
        con.setRequestProperty("Accept", "application/json");
        con.setDoOutput(true);



        // Send post request
        con.connect();
        OutputStream os = con.getOutputStream();
        os.write(bytes);
        os.close();

        int responseCode = con.getResponseCode();
        System.out.println("\nSending 'POST' request to URL : " + url);
        //System.out.println("Post parameters : " + json);
        System.out.println("Response Code : " + responseCode);

        BufferedReader in = new BufferedReader(
                new InputStreamReader(con.getInputStream()));
        String inputLine;
        StringBuffer response = new StringBuffer();

        while ((inputLine = in.readLine()) != null) {
            response.append(inputLine);
        }
        in.close();

        //print result
        System.out.println(response.toString());

    }

}
