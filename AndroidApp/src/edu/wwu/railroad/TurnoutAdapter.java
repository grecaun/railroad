package edu.wwu.railroad;

import android.app.FragmentManager;
import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;

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
 * Adapter for displaying turnouts.
 *
 * @author James Sentinella
 * @version 1.0
 */
public class TurnoutAdapter extends BaseAdapter {
	private static final String TAG = "TurnoutAdapter";

	// This variable should be changed based upon the layout.
	private static final int NUMTURNOUTS = 23;
	// Used to inflate views.
	private static LayoutInflater  inflater;
	// Used to send us back to the connection fragment if the socket closes.
	private static FragmentManager fragMan;

	/**
	 * Constructor.
	 *
	 * @param context Context.
	 * @param fm Fragment manager.
	 */
	public TurnoutAdapter(Context context, FragmentManager fm) {
		fragMan = fm;
		inflater = LayoutInflater.from(context);
	}

	@Override
	public int getCount() {
		return NUMTURNOUTS;
	}

	@Override
	public Object getItem(int index) {
		return index;
	}

	@Override
	public long getItemId(int index) {
		return index;
	}

	@Override
	public View getView(int index, View convView, ViewGroup parent) {
		View outView = convView;
		if (outView == null) {
			outView = inflater.inflate(R.layout.turnout_item, null);
		}
		TextView label = (TextView) outView.findViewById(R.id.turnoutLabel);

		label.setText("Turnout "+(index+1));

		Button throwBtn = (Button) outView.findViewById(R.id.turnoutThrow);
		Button closeBtn = (Button) outView.findViewById(R.id.turnoutClose);

		throwBtn.setTag(index+1);
		closeBtn.setTag(index+1);

		throwBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				int num = (Integer) v.getTag();
				Log.v(TAG, "onClick throw, turnout # "+num);
				SockInfo.out.println("[TOT|"+num+"|T]");
				if (SockInfo.checkSock()){
					fragMan.beginTransaction()
		            	.replace(R.id.container, new MainFragment())
		            	.commit();
				}
			}
		});

		closeBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				int num = (Integer) v.getTag();
				Log.v(TAG, "onClick close, turnout # "+num);
				SockInfo.out.println("[TOT|"+num+"|C]");
				if (SockInfo.checkSock()){
					fragMan.beginTransaction()
		            	.replace(R.id.container, new MainFragment())
		            	.commit();
				}
			}
		});
		return outView;
	}

}
