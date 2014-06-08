package edu.wwu.railroad;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.app.Fragment;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
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
	private static final String       TAG  = "MainFragment";
	private static       String       port = "54320";
	private static       String       ip   = "";
	
    private static final Pattern tokenPattern = Pattern.compile("[^\\[]*?\\[([^\\[\\]\\|]*)\\|([^\\[\\]\\|]*)\\]");

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
		EditText ipAdd  = (EditText)rootView.findViewById(R.id.connectIP);
		ipAdd.addTextChangedListener(new TextWatcher() {

			@Override
			public void afterTextChanged(Editable text) {
				ip = text.toString();
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count, int after) {}

			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {}
			
		});
		if (ip.length() > 0) {
			ipAdd.setText(ip);
		}
		
		EditText portNo = (EditText)rootView.findViewById(R.id.connectPort);
		portNo.addTextChangedListener(new TextWatcher() {
			
			@Override
			public void afterTextChanged(Editable text) {
				port = text.toString();
			}

			@Override
			public void beforeTextChanged(CharSequence arg0, int arg1, int arg2, int arg3) {}

			@Override
			public void onTextChanged(CharSequence arg0, int arg1, int arg2, int arg3) {}
			
		});
		if (port.length() > 0) {
			portNo.setText(port);
		}
		
        return rootView;
    }

	@Override
	public void onClick(View aView) {
		// Get the IP the user has typed in and pass it to a new thread.
		EditText ipBox   = (EditText) getView().findViewById(R.id.connectIP);
		EditText portBox = (EditText) getView().findViewById(R.id.connectPort);
		Log.d(TAG, "Attempting to connect to "+ipBox.getText()+" with port number "+portBox.getText());
		new ConnectionTask(this).execute(ipBox.getText().toString(),portBox.getText().toString());
	}
	
	/**
	 * Method that tells the fragment to change the view to the controls view.
	 */
	public void showControls() {
		if (SockInfo.socket != null) {
			ConFrag newFrag = new ConFrag();
			getFragmentManager().beginTransaction().replace(R.id.container, newFrag).commit();
		} else {
			Toast.makeText(getActivity(), "Unable to Connect.", Toast.LENGTH_SHORT).show();
		}
	}
	
	/**
	 * Method that tells the fragment to show the verification view.
	 */
	public void showVerification() {
		if (SockInfo.socket == null) { 
			Toast.makeText(getActivity(), "Unable to Connect.", Toast.LENGTH_SHORT).show();
			return;
		}
		Log.d(TAG, "About to set the new fragment...");
		VerificationFragment vf  = new VerificationFragment();
		getFragmentManager().beginTransaction().replace(R.id.container, vf).commit();
	}

    /**
     * Small runnable object for establishing a socket connection
     * with the API.
     *
     * @author James Sentinella
     * @version 1.0
     */
    public static class ConnectionTask extends AsyncTask<String, Void, Void> {
    	private MainFragment frag;
    	private boolean      verify = false;

    	public ConnectionTask(MainFragment caller){
    		this.frag = caller;
    	}

		@Override
		protected Void doInBackground(String... params) {
			InetAddress serverAddr;
			try {
				serverAddr = InetAddress.getByName(params[0]);
				int portNumber = Integer.parseInt(params[1]);
				SockInfo.socket = new Socket(serverAddr, portNumber);
				SockInfo.out = new PrintWriter(new OutputStreamWriter(SockInfo.socket.getOutputStream()), true);
				// Check if we need to verify ourself.
				BufferedReader input = new BufferedReader(new InputStreamReader(SockInfo.socket.getInputStream()));
				String received = input.readLine();
				Log.d(TAG, "Input received: "+received);
				Matcher tokenMatcher = tokenPattern.matcher(received);
				if (tokenMatcher.find()) {
					String command = tokenMatcher.group(1);
					String state   = tokenMatcher.group(2);
					Log.d(TAG, "Command: "+command+" state: "+state);
					if (command.equalsIgnoreCase("ATOKEN") &&
					    state.equalsIgnoreCase("RQS")) {
						verify = true;
					}
				}
			} catch (UnknownHostException e) {
				SockInfo.socket = null;
			} catch (IOException e) {
				SockInfo.socket = null;
			} catch (Exception e) {
				SockInfo.socket = null;
				Log.e(TAG, "Something went wrong. Most likely insufficient arguments given to execute method.");
			}
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			if (verify) {
				frag.showVerification();
			} else {
				frag.showControls();
			}
		}

    }
    
}