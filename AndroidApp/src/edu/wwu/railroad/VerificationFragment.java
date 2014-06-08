package edu.wwu.railroad;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.app.Fragment;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
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
 * Class used to display verification fragment.
 * 
 * @author James Sentinella
 * @version 1.0
 */
public class VerificationFragment extends Fragment implements OnClickListener {
    private static final Pattern tokenPattern = Pattern.compile("[^\\[]*?\\[([^\\[\\]\\|]*)\\|([^\\[\\]\\|]*)\\]");
	
	@Override
	public void onCreate(Bundle aSavedInstanceState) {
		super.onCreate(aSavedInstanceState);
		this.setRetainInstance(true);
	}
	
	@Override
    public View onCreateView(LayoutInflater aInflater, ViewGroup aContainer,
            Bundle aSavedInstanceState) {
		View rootView = aInflater.inflate(R.layout.fragment_verification, aContainer, false);
		rootView.findViewById(R.id.tokenOkay).setOnClickListener(this);
		rootView.findViewById(R.id.tokenCancel).setOnClickListener(this);
		return rootView;
	}

	@Override
	public void onClick(View v) {
		if (v.getId() == R.id.tokenOkay) {
			EditText tokenET = (EditText) getView().findViewById(R.id.tokenBox);
			new VerificationTask(this).execute(tokenET.getText().toString());
		} else if (v.getId() == R.id.tokenCancel) {
			SockInfo.close();
			MainFragment newFrag = new MainFragment();
			getFragmentManager().beginTransaction().replace(R.id.container, newFrag).commit();
		}
	}
	
	public void showControls() {
		if (SockInfo.socket != null) {
			ConFrag newFrag = new ConFrag();
			getFragmentManager().beginTransaction().replace(R.id.container, newFrag).commit();
		} else {
			Toast.makeText(getActivity(), "Unable to Connect.", Toast.LENGTH_SHORT).show();
		}
	}
	
	/**
	 * Used to send a verification token in the background
	 * and update the fragment the user views if it send the
	 * correct token.
	 * 
	 * @author James Sentinella
	 * @version 1.0
	 */
	public static class VerificationTask extends AsyncTask<String, Void, Void> {
		private static final String TAG = "VerificationTask";
		
		private boolean verified = false;
		private VerificationFragment frag;
		
		public VerificationTask(VerificationFragment caller){
    		this.frag = caller;
    	}
		
		@Override
		protected Void doInBackground(String... params) {
			try {
				SockInfo.out.println("[CTOKEN|"+params[0]+"]");
				BufferedReader input = new BufferedReader(new InputStreamReader(SockInfo.socket.getInputStream()));
				String received = input.readLine();
				Log.d(TAG, "Input received: "+received);
				Matcher tokenMatcher = tokenPattern.matcher(received);
				if (tokenMatcher.find()) {
					String command = tokenMatcher.group(1);
					String state   = tokenMatcher.group(2);
					if (command.equalsIgnoreCase("ATOKEN") &&
					    state.equalsIgnoreCase("ACC")) {
						verified = true;
					}
				}
			} catch (Exception e) {
				Log.e(TAG, "Unable to complete verification.");
			}
			return null;
		}
		
		@Override
		protected void onPostExecute(Void result) {
			if (verified) {
				frag.showControls();
			}			
		}
		
	}
}
