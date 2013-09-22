import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.ObjectWriter;
import java.util.List;


public class Main {

    public static void main(String[] args) {

        try{
            Database database = new Database();
            List<Day> days =  database.doSql();

            ObjectWriter ow = new ObjectMapper().writer().withDefaultPrettyPrinter();
            byte[] jsonBytes = ow.writeValueAsBytes(days);

            Net net = new Net();
            net.sendPost(jsonBytes);

        } catch (Exception e){
            e.printStackTrace();
        }

    }
}
