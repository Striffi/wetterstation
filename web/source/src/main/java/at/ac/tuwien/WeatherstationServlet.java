package at.ac.tuwien;

import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class WeatherstationServlet extends HttpServlet {

	private static final long serialVersionUID = 1L;

	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
		List<String[]> measurements = new ArrayList<String[]>();
		try {
			if (request.getParameter("type") != null) {
				measurements = WeatherstationDAO.getInstance().retrieveMeasurements(request.getParameter("type"));
			}
			
			Collections.reverse(measurements);
			response.getWriter().write(flatten(measurements));
			response.getWriter().flush();
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	private String flatten(List<String[]> measurements) {
		StringBuilder sb = new StringBuilder();
		Iterator<String[]> iter = measurements.iterator();
		if (iter.hasNext()) {
			append(sb, iter.next());
			while (iter.hasNext()) {
				sb.append(";");
				append(sb, iter.next());
			}
		}
		return sb.toString();
	}

	private void append(StringBuilder sb, String[] values) {
		sb.append(values[0]);
		sb.append(",");
		sb.append(values[1]);
	}
}
