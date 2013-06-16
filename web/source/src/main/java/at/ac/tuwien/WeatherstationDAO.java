package at.ac.tuwien;

import java.io.FileInputStream;
import java.io.IOException;
import java.math.BigDecimal;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Properties;

public class WeatherstationDAO {

	private Connection con;

	private static WeatherstationDAO instance;

	public static WeatherstationDAO getInstance() {
		if (instance == null) {
			instance = new WeatherstationDAO();
		}
		return instance;
	}

	public WeatherstationDAO() {
		Properties prop = new Properties();
		try {
			prop.load(new FileInputStream("weatherstation.properties"));
			String dbhost = prop.getProperty("dbhost", "localhost");
			String dbport = prop.getProperty("dbport", "5432");
			String db = prop.getProperty("db", "weatherstation");
			String dbuser = prop.getProperty("dbuser", "postgres");
			String dbpass = prop.getProperty("dbpass", "password");
			Class.forName("org.postgresql.Driver");
			con = DriverManager.getConnection("jdbc:postgresql://" + dbhost + ":" + dbport + "/" + db + "", dbuser,
					dbpass);
			con.setAutoCommit(true);
			if (con == null) {
				System.err.println("Failed to make connection!");
			}
		} catch (SQLException e) {
			System.out.println("Connection Failed! Check output console");
			e.printStackTrace();

		} catch (ClassNotFoundException e) {
			System.out.println("Where is your PostgreSQL JDBC Driver? " + "Include in your library path!");
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public List<String[]> retrieveMeasurements(String type) throws SQLException {
		List<String[]> measurements = new ArrayList<String[]>();
		if (con == null) {
			return measurements;
		}
		type = type.substring(0, 1).toUpperCase() + type.substring(1);
		Statement stmt = con.createStatement();
		String query = "SELECT time, measurement FROM weathervalues where sensor='" + type
				+ "' order by time desc LIMIT 50;";
		ResultSet rs = stmt.executeQuery(query);
		while (rs.next()) {
			measurements.add(new String[] { rs.getString(1), rs.getString(2) });
		}
		if (measurements.size() == 0) {
			measurements.add(new String[] { "0", "0" });
		}
		measurements.add(new String[] { "Time",  type});
		return measurements;
	}

	public void insertMeasurement(BigDecimal measurement, String type) throws SQLException {
		if (con == null) {
			return;
		}
		String query = "INSERT INTO weathervalues (time, measurement, sensor) values (?,?,?)";
		PreparedStatement stmt = con.prepareStatement(query);
		stmt.setTimestamp(1, new Timestamp(new Date().getTime()));
		stmt.setBigDecimal(2, measurement);
		stmt.setString(3, type);
		stmt.execute();
	}
}
