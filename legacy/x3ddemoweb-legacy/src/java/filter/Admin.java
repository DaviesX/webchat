/*
 * Copyright (C) 2017 davis
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
package filter;

import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;

/**
 *
 * @author davis
 */
public class Admin implements Filter {
        
        private static final boolean DEBUG = true;

        // The filter configuration object we are associated with.  If
        // this value is null, this filter instance is not currently
        // configured. 
        private FilterConfig filterConfig = null;
        
        public Admin() {
        }        
        
        /**
         *
         * @param request The servlet request we are processing
         * @param response The servlet response we are creating
         * @param chain The filter chain we are processing
         *
         * @exception IOException if an input/output error occurs
         * @exception ServletException if a servlet error occurs
         */
        @Override
        public void doFilter(ServletRequest request, ServletResponse response,
                             FilterChain chain)
                throws IOException, ServletException {
                
                if (DEBUG) {
                        log("Admin:doFilter()");
                }
                
                request.getRequestDispatcher("401.jsp").forward(request, response);
                
                Throwable problem = null;
                try {
                        chain.doFilter(request, response);
                } catch (IOException | ServletException t) {
                        // If an exception is thrown somewhere down the filter chain,
                        // we still want to execute our after processing, and then
                        // rethrow the problem after that.
                        problem = t;
                }
                

                // If there was a problem, we want to rethrow it if it is
                // a known type, otherwise log it.
                if (problem != null) {
                        if (problem instanceof ServletException) {
                                throw (ServletException) problem;
                        }
                        if (problem instanceof IOException) {
                                throw (IOException) problem;
                        }
                        sendProcessingError(problem, response);
                }
        }

        /**
         * Return the filter configuration object for this filter.
         * @return 
         */
        public FilterConfig getFilterConfig() {
                return (this.filterConfig);
        }

        /**
         * Set the filter configuration object for this filter.
         *
         * @param filterConfig The filter configuration object
         */
        public void setFilterConfig(FilterConfig filterConfig) {
                this.filterConfig = filterConfig;
        }

        /**
         * Destroy method for this filter
         */
        @Override
        public void destroy() {                
        }

        /**
         * Init method for this filter
         * @param filterConfig
         */
        @Override
        public void init(FilterConfig filterConfig) {                
                this.filterConfig = filterConfig;
                if (filterConfig != null) {
                        if (DEBUG) {                                
                                log("Admin:Initializing filter");
                        }
                }
        }

        /**
         * Return a String representation of this object.
         */
        @Override
        public String toString() {
                if (filterConfig == null) {
                        return ("Admin()");
                }
                StringBuilder sb = new StringBuilder("Admin(");
                sb.append(filterConfig);
                sb.append(")");
                return (sb.toString());
        }
        
        private void sendProcessingError(Throwable t, ServletResponse response) {
                String stackTrace = getStackTrace(t);                
                
                if (stackTrace != null && !stackTrace.equals("")) {
                        try {
                                response.setContentType("text/html");
                                try (PrintStream ps = new PrintStream(response.getOutputStream()); 
                                     PrintWriter pw = new PrintWriter(ps)) {
                                        pw.print("<html>\n<head>\n<title>Error</title>\n</head>\n<body>\n"); //NOI18N
                                        
                                        // PENDING! Localize this for next official release
                                        pw.print("<h1>The resource did not process correctly</h1>\n<pre>\n");
                                        pw.print(stackTrace);
                                        pw.print("</pre></body>\n</html>"); //NOI18N
                                }
                                response.getOutputStream().close();
                        } catch (Exception ex) {
                        }
                } else {
                        try {
                                try (PrintStream ps = new PrintStream(response.getOutputStream())) {
                                        t.printStackTrace(ps);
                                }
                                response.getOutputStream().close();
                        } catch (Exception ex) {
                        }
                }
        }
        
        public static String getStackTrace(Throwable t) {
                String stackTrace = null;
                try {
                        StringWriter sw = new StringWriter();
                        PrintWriter pw = new PrintWriter(sw);
                        t.printStackTrace(pw);
                        pw.close();
                        sw.close();
                        stackTrace = sw.getBuffer().toString();
                } catch (Exception ex) {
                }
                return stackTrace;
        }
        
        public void log(String msg) {
                filterConfig.getServletContext().log(msg);                
        }
        
}
