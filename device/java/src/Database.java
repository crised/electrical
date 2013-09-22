import java.sql.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: crised
 * Date: 9/22/13
 * Time: 3:47 PM
 * To change this template use File | Settings | File Templates.
 */
public class Database {

    private Connection connection = null;
    private Statement statement = null;

    private String username = "crised";
    private String password = "password";

    private String dbURL = "jdbc:postgresql://127.0.0.1:5432/meter";
    private String sql = "SELECT * from values";

    private List<Day> days;

    public List<Day> doSql() {


        try {

            connection = DriverManager.getConnection(dbURL, username, password);

            statement = connection.createStatement();
            ResultSet resultSet = statement.executeQuery(sql);

            days = new ArrayList<>();

            while (resultSet.next()) {
                Day day = new Day();
                day.setId(resultSet.getInt("id"));
                day.setName(resultSet.getString("name"));
                days.add(day);
            }

            for (Day d : days) {
                System.out.println(d.getId() + " , " + d.getName());
            }


            resultSet.close();
            statement.close();
            connection.close();

            return days;

        } catch (SQLException e) {

            System.out.println("Connection Failed! Check output console");
            e.printStackTrace();
            return null;

        }
    }
}
