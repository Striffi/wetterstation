package at.ac.tuwien;

import java.io.FileInputStream;
import java.util.Properties;

import org.eclipse.jetty.server.Connector;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.nio.SelectChannelConnector;
import org.eclipse.jetty.servlet.ServletHandler;
import org.eclipse.jetty.webapp.WebAppContext;

public class WeatherstationServer {

	public static void main(String... args) throws Exception {
		Server server = new Server();
		Connector connector = new SelectChannelConnector();

		Properties prop = new Properties();
		prop.load(new FileInputStream("weatherstation.properties"));
		
		connector.setPort(Integer.valueOf(prop.getProperty("port", "8081")));
		connector.setHost(prop.getProperty("host", "localhost"));
		server.addConnector(connector);

		WebAppContext wac = new WebAppContext();
		wac.setWar("webapp");

		ServletHandler servletHandler = new ServletHandler();
		servletHandler.addServletWithMapping(WeatherstationServlet.class, "/weatherstation");
		wac.setServletHandler(servletHandler);
		server.setHandler(wac);

		server.setStopAtShutdown(true);

		try {
		/*
		 * Insert test data
		 */
		/*
			
			new Thread(new Runnable() {

				@Override
				public void run() {
					Random rand = new Random(199);
					while (true) {
						try {
							Thread.sleep(1000);
							WeatherstationDAO.getInstance().insertMeasurement(new BigDecimal(rand.nextInt(100)), "humidity");
							WeatherstationDAO.getInstance().insertMeasurement(new BigDecimal(rand.nextInt(100)), "pressure");
							WeatherstationDAO.getInstance().insertMeasurement(new BigDecimal(rand.nextInt(100)), "temperature");
						} catch (SQLException e) {
							e.printStackTrace();
						} catch (InterruptedException e) {
//							e.printStackTrace();
						}
					}
				}
			}).start();
			*/
		
			server.start();
			server.join();
		} catch (Exception e) {
			e.printStackTrace(System.out);
		} finally {
			server.stop();
		}
	}
}
