package edu.wwu.railroad;

import android.app.Activity;
import android.os.Bundle;

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
 * Main activity.  Pretty dull actually.
 *
 * @author James Sentinella
 * @version 1.0
 */
public class MainActivity extends Activity {
    private static final String MAINFRAGTAG = "MAINFRAG";

	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new MainFragment(), MAINFRAGTAG)
                    .commit();
        }

    }

}
