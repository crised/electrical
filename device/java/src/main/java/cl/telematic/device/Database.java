package cl.telematic.device;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Database {

    private Connection connection = null;

    private PreparedStatement removeDemandStatsStatement;

    private PreparedStatement removeEnergyStatsStatement;

    private PreparedStatement removeInstantStatsStatement;

    private PreparedStatement selectDemandStatsStatement;

    private PreparedStatement selectEnergyStatsStatement;

    private PreparedStatement selectInstantStatsStatement;

    public List<LocalDemandStats> getDemandStats() throws SQLException
    {
        final ResultSet resultSet = getSelectDemandStatsStatement().executeQuery();
        if (!resultSet.isBeforeFirst()) {
            return Collections.emptyList();
        }
        final List<LocalDemandStats> results = new ArrayList<>();
        while (resultSet.next()) {
            final LocalDemandStats stats = new LocalDemandStats();
            stats.setId(resultSet.getLong(1));
            stats.setCreatedOn(resultSet.getDate(2));
            stats.setKw_import_block_demand(resultSet.getLong(3));
            stats.setKvar_import_block_demand(resultSet.getLong(4));
            stats.setKva_block_demand(resultSet.getLong(5));
            results.add(stats);
        }
        resultSet.close();
        return results;
    }

    public List<LocalEnergyStats> getEnergyStats() throws SQLException
    {
        final ResultSet resultSet = getSelectEnergyStatsStatement().executeQuery();
        if (!resultSet.isBeforeFirst()) {
            return Collections.emptyList();
        }
        final List<LocalEnergyStats> results = new ArrayList<>();
        while (resultSet.next()) {
            final LocalEnergyStats stats = new LocalEnergyStats();
            stats.setId(resultSet.getLong(1));
            stats.setCreatedOn(resultSet.getDate(2));
            stats.setKwh_import(resultSet.getLong(3));
            stats.setKwh_import_l1(resultSet.getLong(4));
            stats.setKwh_import_l2(resultSet.getLong(5));
            stats.setKwh_import_l3(resultSet.getLong(6));
            results.add(stats);
        }
        resultSet.close();
        return results;
    }

    public List<LocalInstantStats> getInstantStats() throws SQLException
    {
        final ResultSet resultSet = getSelectInstantStatsStatement().executeQuery();
        if (!resultSet.isBeforeFirst()) {
            return Collections.emptyList();
        }
        final List<LocalInstantStats> results = new ArrayList<>();
        while (resultSet.next()) {
            final LocalInstantStats stats = new LocalInstantStats();
            stats.setId(resultSet.getLong(1));
            stats.setCreatedOn(resultSet.getDate(2));
            stats.setTotal_kw(resultSet.getLong(3));
            stats.setTotal_pf(resultSet.getLong(4));
            stats.setV1_voltage(resultSet.getLong(5));
            stats.setV2_voltage(resultSet.getLong(6));
            stats.setV3_voltage(resultSet.getLong(7));
            results.add(stats);
        }
        resultSet.close();
        return results;
    }

    public void removeDemandStats(List<LocalDemandStats> statses) throws SQLException
    {
        remove(statses, getRemoveDemandStatsStatement());
    }

    public void removeEnergyStats(List<LocalEnergyStats> statses) throws SQLException
    {
        remove(statses, getRemoveEnergyStatsStatement());
    }

    public void removeInstantStats(List<LocalInstantStats> statses) throws SQLException
    {
        remove(statses, getRemoveInstantStatsStatement());
    }

    @Override
    protected void finalize() throws Throwable
    {
        if (null != selectDemandStatsStatement) {
            try {
                selectDemandStatsStatement.close();
            } catch (SQLException e) {
                System.err.println("Cannot close selectDemandStatsStatement");
                e.printStackTrace();
            }
        }
        if (null != selectEnergyStatsStatement) {
            try {
                selectEnergyStatsStatement.close();
            } catch (SQLException e) {
                System.err.println("Cannot close selectEnergyStatsStatement");
                e.printStackTrace();
            }
        }
        if (null != selectInstantStatsStatement) {
            try {
                selectInstantStatsStatement.close();
            } catch (SQLException e) {
                System.err.println("Cannot close selectInstantStatsStatement");
                e.printStackTrace();
            }
        }
        if (null != removeDemandStatsStatement) {
            try {
                removeDemandStatsStatement.close();
            } catch (SQLException e) {
                System.err.println("Cannot close removeDemandStatsStatement");
                e.printStackTrace();
            }
        }
        if (null != removeEnergyStatsStatement) {
            try {
                removeEnergyStatsStatement.close();
            } catch (SQLException e) {
                System.err.println("Cannot close removeEnergyStatsStatement");
                e.printStackTrace();
            }
        }
        if (null != removeInstantStatsStatement) {
            try {
                removeInstantStatsStatement.close();
            } catch (SQLException e) {
                System.err.println("Cannot close removeInstantStatsStatement");
                e.printStackTrace();
            }
        }
        if (null != connection) {
            try {
                connection.close();
            } catch (SQLException e) {
                System.err.println("Cannot close connection");
                e.printStackTrace();
            }
        }
        super.finalize();
    }

    private Connection getConnection() throws SQLException
    {
        if (null == connection) {
            String username = "postgres";
            String password = "";
            String dbURL = "jdbc:postgresql://127.0.0.1:5432/cenergy";
            connection = DriverManager.getConnection(dbURL, username, password);
            connection.setAutoCommit(true);
        }
        return connection;
    }

    private PreparedStatement getRemoveDemandStatsStatement() throws SQLException
    {
        if (null == removeDemandStatsStatement) {
            removeDemandStatsStatement = getConnection().prepareStatement("UPDATE demand SET sent=TRUE WHERE id = ?");
        }
        return removeDemandStatsStatement;
    }

    private PreparedStatement getRemoveEnergyStatsStatement() throws SQLException
    {
        if (null == removeEnergyStatsStatement) {
            removeEnergyStatsStatement = getConnection().prepareStatement("UPDATE energy SET sent=TRUE WHERE id = ?");
        }
        return removeEnergyStatsStatement;
    }

    private PreparedStatement getRemoveInstantStatsStatement() throws SQLException
    {
        if (null == removeInstantStatsStatement) {
            removeInstantStatsStatement = getConnection().prepareStatement("UPDATE instant SET sent=TRUE WHERE id = ?");
        }
        return removeInstantStatsStatement;
    }

    private PreparedStatement getSelectDemandStatsStatement() throws SQLException
    {
        if (null == selectDemandStatsStatement) {
            final String sql = "SELECT id,datetime,kw_import_block_demand,kvar_import_block_demand,kva_block_demand FROM demand WHERE sent=FALSE order by datetime asc";
            selectDemandStatsStatement = getConnection().prepareStatement(sql);
        }
        return selectDemandStatsStatement;
    }

    private PreparedStatement getSelectEnergyStatsStatement() throws SQLException
    {
        if (null == selectEnergyStatsStatement) {
            final String sql = "SELECT id,datetime,kwh_import,kwh_import_l1,kwh_import_l2,kwh_import_l3 FROM energy WHERE sent=FALSE order by datetime asc";
            selectEnergyStatsStatement = getConnection().prepareStatement(sql);
        }
        return selectEnergyStatsStatement;
    }

    private PreparedStatement getSelectInstantStatsStatement() throws SQLException
    {
        if (null == selectInstantStatsStatement) {
            final String sql = "SELECT id,datetime,total_kw,total_pf,v1_voltage,v2_voltage,v3_voltage FROM instant WHERE sent=FALSE order by datetime asc";
            selectInstantStatsStatement = getConnection().prepareStatement(sql);
        }
        return selectInstantStatsStatement;
    }

    private void remove(List<? extends Identifiable> statses, PreparedStatement statement) throws SQLException
    {
        for (Identifiable stats : statses) {
            statement.setLong(1, stats.getId());
            statement.executeUpdate();
        }
    }
}
