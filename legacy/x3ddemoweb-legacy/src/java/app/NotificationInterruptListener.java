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
package app;

import com.sun.corba.se.impl.orbutil.concurrent.Mutex;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author davis
 */
public class  NotificationInterruptListener implements backend.NotificationListener {
        
        private final ArrayList<backend.Notification>   m_notifications;
        private boolean                                 m_has_started = true;
        private final Mutex                             m_mutex;
        
        public NotificationInterruptListener() {
                m_mutex = new Mutex();
                m_notifications = new ArrayList<>();
        }
        
        private boolean is_hold_condition() {
                return !m_has_started || m_notifications.isEmpty();
        }
        
        public ArrayList<backend.Notification> fetch_notifications() {
                try {
                        boolean ret_val;
                        m_mutex.acquire();
                        ret_val = !is_hold_condition();
                        m_mutex.release();
                        return ret_val ? m_notifications : null;
                } catch (InterruptedException ex) {
                        Logger.getLogger(NotificationInterruptListener.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
	
	@Override
	public void hold() {
		try {
			m_mutex.acquire();
			m_has_started = false;
			m_mutex.release();
		} catch (InterruptedException ex) {
			Logger.getLogger(NotificationInterruptListener.class.getName()).log(Level.SEVERE, null, ex);
		}
	}
        
        @Override
        public void release() {
                try {
                        m_mutex.acquire();
                        m_has_started = true;
                        m_mutex.release();
                } catch (InterruptedException ex) {
                        Logger.getLogger(NotificationInterruptListener.class.getName()).log(Level.SEVERE, null, ex);
                }
        }

        @Override
        public boolean on_receive(backend.Notification noti) {
                try {
                        boolean ret_val;
                        m_mutex.acquire();
                        ret_val = is_hold_condition();
                        if (ret_val) {
                                m_notifications.add(noti);
                        }
                        m_mutex.release();
                        return ret_val;
                } catch (InterruptedException ex) {
                        Logger.getLogger(NotificationInterruptListener.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
}
