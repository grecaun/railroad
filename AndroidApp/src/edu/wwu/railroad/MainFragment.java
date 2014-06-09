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
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
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
	private static final String TAG      = "MainFragment";
	private static final String PORTPREF = "PORT";
	private static final String IPPREF   = "IPADDRESS";
	private              String port     = "54320";
	private              String ip       = "";
	
	private SharedPreferences pPrefs;
	
    private static final Pattern tokenPattern = Pattern.compile("[^\\[]*?\\[([^\\[\\]\\|]*)\\|([^\\[\\]\\|]*)\\]");

	@Override
	public void onCreate(Bundle aSavedInstanceState) {
		super.onCreate(aSavedInstanceState);
		this.setRetainInstance(true);
	}

    @Override
    public View onCreateView(LayoutInflater aInflater, ViewGroup aContainer,
            Bundle aSavedInstanceState) {
    	pPrefs = getActivity().getPreferences(Context.MODE_PRIVATE);
    	port = pPrefs.getString(PORTPREF, "");
    	ip   = pPrefs.getString(IPPREF, "");
    	if (port.length() < 1) {
    		Editor edit = pPrefs.edit();
    		port = "54320";
    		edit.putString("PORTNO", port);
    		edit.commit();
    	}
        View rootView = aInflater.inflate(R.layout.fragment_main, aContainer, false);
		Button conBtn = (Button)rootView.findViewById(R.id.connectButton);
		conBtn.setOnClickListener(this);
		EditText ipAdd  = (EditText)rootView.findViewById(R.id.connectIP);
		ipAdd.addTextChangedListener(new TextWatcher() {

			@Override
			public void afterTextChanged(Editable text) {
				ip = text.toString();
				Editor edit = pPrefs.edit();
				edit.putString(IPPREF, ip);
				edit.commit();
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
				Editor edit = pPrefs.edit();
				edit.putString(PORTPREF, port);
				edit.commit();
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
    	private int          verify = -1; // 1 = verify, 0 = don't, -1 implies
    									  // no message received
    	private Socket       pSock;

    	public ConnectionTask(MainFragment caller){
    		this.frag = caller;
    	}

		@Override
		protected Void doInBackground(String... params) {
			synchronized (frag) {
				InetAddress serverAddr;
				if (frag == null || SockInfo.socket != null) { return null; }
				try {
					serverAddr = InetAddress.getByName(params[0]);
					int portNumber = Integer.parseInt(params[1]);
					SockInfo.socket = new Socket(serverAddr, portNumber);
					SockInfo.socket.setSoTimeout(1000);
					pSock = SockInfo.socket;
					SockInfo.out = new PrintWriter(new OutputStreamWriter(SockInfo.socket.getOutputStream()), true);
				} catch (UnknownHostException e) {
					SockInfo.socket = null;
				} catch (IOException e) {
					SockInfo.socket = null;
				} catch (Exception e) {
					SockInfo.socket = null;
					Log.e(TAG, "Something went wrong. Most likely insufficient arguments given to execute method.");
				}
				if (SockInfo.socket != null) {
					try {
						// Check if we need to verify ourself.
						BufferedReader input = new BufferedReader(new InputStreamReader(SockInfo.socket.getInputStream()));
						String received = input.readLine();
						Log.d(TAG, "Input received: "+received);
						Matcher tokenMatcher = tokenPattern.matcher(received);
						if (tokenMatcher.find()) {
							String command = tokenMatcher.group(1);
							String state   = tokenMatcher.group(2);
							Log.d(TAG, "Command: "+command+" state: "+state);
							if (command.equalsIgnoreCase("ATOKEN")) {
								if (state.equalsIgnoreCase("RQS")) {
									verify = 1;
								} else if (state.equalsIgnoreCase("NTK")) {
									verify = 0;
								}
							}
						}
					} catch (Exception e) {
						Log.e(TAG, "No message from API. Most likely non verifying.");
					}
				}
			}
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			if (frag == null || pSock != SockInfo.socket) { return; }
			if (verify == 1) {
				frag.showVerification();
			} else if (verify == 0){
				frag.showControls();
			} else {
				if (SockInfo.socket != null) {
					Log.e(TAG, "API didn't respond/didn't send token command."
							+ " Assuming non validating API.");
				}
				frag.showControls();
			}
		}

    }
    
}