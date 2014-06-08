package edu.wwu.railroad;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.GridView;
import android.widget.SeekBar;
import android.widget.Toast;
import android.widget.SeekBar.OnSeekBarChangeListener;

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
 * Fragment that shows controls for controlling trains.
 *
 * @author James Sentinella
 * @version 1.0
 */
public class ConFrag extends Fragment {

     @Override
     public View onCreateView(LayoutInflater inflater, ViewGroup container,
             Bundle savedInstanceState) {
    	 // Make a new rootView.
		 View rootView = inflater.inflate(R.layout.fragment_connected, container, false);
		 // Get the gridview for displaying turnouts and set the adapter.
		 GridView gView = (GridView) rootView.findViewById(R.id.turnoutGrid);
		 gView.setAdapter(new TurnoutAdapter(getActivity(), getFragmentManager()));

		 // Make a listener for buttons.
		 View.OnClickListener buttonListener = new View.OnClickListener() {
			@Override
			public void onClick(View view) {
				switch (view.getId()) {
					case R.id.powon:
						SockInfo.out.println("[POW|ON]");
						break;
					case R.id.powoff:
						SockInfo.out.println("[POW|OFF]");
						break;
					case R.id.exit:
						SockInfo.out.println("[POW|OFF]");
						SockInfo.out.println("[EXT]");
						break;
					case R.id.trainOneEStp:
						SockInfo.out.println("[STP|1]");
						break;
					case R.id.trainOneFwd:
						SockInfo.out.println("[DIR|1|F]");
						break;
					case R.id.trainOneRev:
						SockInfo.out.println("[DIR|1|R]");
						break;
					case R.id.trainOneSnd:
						SockInfo.out.println("[SND|1|"+((Integer)view.getTag() == 1 ? "OFF]":"ON]"));
						view.setTag(((Integer)view.getTag() == 1 ? 0:1));
						break;
					case R.id.trainTwoEStp:
						SockInfo.out.println("[STP|3]");
						break;
					case R.id.trainTwoFwd:
						SockInfo.out.println("[DIR|3|F]");
						break;
					case R.id.trainTwoRev:
						SockInfo.out.println("[DIR|3|R]");
						break;
					case R.id.trainTwoSnd:
						SockInfo.out.println("[SND|3|"+((Integer)view.getTag() == 1 ? "OFF]":"ON]"));
						view.setTag(((Integer)view.getTag() == 1 ? 0:1));
						break;
				}
				// Check to make sure we're still connected.
				if (SockInfo.checkSock()){
					getFragmentManager().beginTransaction()
		            	.replace(R.id.container, new MainFragment())
		            	.commit();
				}
			}
		 };

		 // Set all the button listeners.
		 Button btn = (Button) rootView.findViewById(R.id.powon);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.powoff);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.exit);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.trainOneEStp);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.trainOneFwd);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.trainOneRev);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.trainOneSnd);
		 btn.setTag(1);    // A tag indicating if sound is on (1) or off (0)
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.trainTwoEStp);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.trainTwoFwd);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.trainTwoRev);
		 btn.setOnClickListener(buttonListener);
		 btn = (Button) rootView.findViewById(R.id.trainTwoSnd);
		 btn.setTag(1);    // see tag above
		 btn.setOnClickListener(buttonListener);

		 // Get the train speed bars and set listeners
		 SeekBar trainOneSpd = (SeekBar) rootView.findViewById(R.id.trainOneSpeed);
		 trainOneSpd.setOnSeekBarChangeListener(new OnSeekBarChangeListener(){
			int progressChanged = 0;
			@Override
			public void onProgressChanged(SeekBar sbar, int progress, boolean fromUser) {
				// update the value tracked
				progressChanged = progress;
			}

			@Override
			public void onStartTrackingTouch(SeekBar arg0) {
				// Do nothing.
			}

			@Override
			public void onStopTrackingTouch(SeekBar arg0) {
				SockInfo.out.println("[SPD|1|"+progressChanged+"]");
				Toast.makeText(getActivity(), "Speed is "+progressChanged, Toast.LENGTH_SHORT).show();
				// Check to make sure we're still connected since.
				if (SockInfo.checkSock()){
					getFragmentManager().beginTransaction()
		            	.replace(R.id.container, new MainFragment())
		            	.commit();
				}
			}
		 });

		 // seee above
		 SeekBar trainTwoSpd = (SeekBar) rootView.findViewById(R.id.trainTwoSpeed);
		 trainTwoSpd.setOnSeekBarChangeListener(new OnSeekBarChangeListener(){
			int progressChanged = 0;
			@Override
			public void onProgressChanged(SeekBar sbar, int progress, boolean fromUser) {
				progressChanged = progress;
			}

			@Override
			public void onStartTrackingTouch(SeekBar arg0) {
				// Do nothing.
			}

			@Override
			public void onStopTrackingTouch(SeekBar arg0) {
				SockInfo.out.println("[SPD|3|"+progressChanged+"]");
				Toast.makeText(getActivity(), "Speed is "+progressChanged, Toast.LENGTH_SHORT).show();
				if (SockInfo.checkSock()){
					getFragmentManager().beginTransaction()
		            	.replace(R.id.container, new MainFragment())
		            	.commit();
				}
			}
		 });

         return rootView;
     }
}
