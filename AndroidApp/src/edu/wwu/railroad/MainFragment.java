package edu.wwu.railroad;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Fragment;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

/*
   This file is part of RailRoad.

   RailRoad is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3 of the License.

   RailRoad is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with RailRoad.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
 * Main Fragment class. This is the fragment from which
 * the user connects to the API.
 *
 * @author James Sentinella
 * @version 1.0
 */
public class MainFragment extends Fragment implements OnClickListener {
	private static final String       TAG = "MainFragment";

	@Override
	public void onCreate(Bundle aSavedInstanceState) {
		super.onCreate(aSavedInstanceState);
		this.setRetainInstance(true);
	}

    @Override
    public View onCreateView(LayoutInflater aInflater, ViewGroup aContainer,
            Bundle aSavedInstanceState) {
        View rootView = aInflater.inflate(R.layout.fragment_main, aContainer, false);
		Button conBtn = (Button)rootView.findViewById(R.id.connectButton);
		conBtn.setOnClickListener(this);
        return rootView;
    }

	@Override
	public void onClick(View aView) {
		// Get the IP the user has typed in and pass it to a new thread.
		EditText ipBox = (EditText) getView().findViewById(R.id.connectIP);
		Log.d(TAG, "Attempting to connect to "+ipBox.getText());
		new ConThread(this).execute(ipBox.getText().toString());
	}

	public void changeView() {
		if (SockInfo.socket != null) {
			ConFrag newFrag = new ConFrag();
			getFragmentManager().beginTransaction().replace(R.id.container, newFrag).commit();
		} else {
			Toast.makeText(getActivity(), "Unable to Connect.", Toast.LENGTH_SHORT).show();
		}
	}

    /**
     * Small runnable object for establishing a socket connection
     * with the API.
     *
     * @author James Sentinella
     * @version 1.0
     */
    static class ConThread extends AsyncTask<String, Void, Void> {
    	MainFragment frag;

    	public ConThread(MainFragment caller){
    		this.frag = caller;
    	}

		@Override
		protected Void doInBackground(String... params) {
			InetAddress serverAddr;
			try {
				serverAddr = InetAddress.getByName(params[0]);
				SockInfo.socket = new Socket(serverAddr, SockInfo.PORTNO);
				SockInfo.out = new PrintWriter(new OutputStreamWriter(SockInfo.socket.getOutputStream()), true);
			} catch (UnknownHostException e) {
				SockInfo.socket = null;
			} catch (IOException e) {
				SockInfo.socket = null;
			}
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			frag.changeView();
		}

    }
}