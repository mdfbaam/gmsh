package org.geuz.onelab;

import android.os.Bundle;
import android.app.Activity;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.CompoundButton;

public class OptionsDisplayFragment extends Fragment{

	private Gmsh _gmsh;
	private SeparatedListView _listView;
	
	public static OptionsDisplayFragment newInstance(Gmsh g) {
		OptionsDisplayFragment fragment = new OptionsDisplayFragment();
		Bundle bundle = new Bundle();
		bundle.putParcelable("Gmsh", g);
		fragment.setArguments(bundle);
        return fragment;
	}
	
	public OptionsDisplayFragment() {
		super();
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		_gmsh = getArguments().getParcelable("Gmsh");
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		_listView = (SeparatedListView)inflater.inflate(R.layout.fragment_options_display, container, false);
		CheckBox showGeomPoints = new CheckBox(_listView.getContext());
		showGeomPoints.setText("Show geometry points");
		showGeomPoints.setChecked((_gmsh.getDoubleOption("Geometry", "Points") > 0.));
		showGeomPoints.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				_gmsh.setDoubleOption("Geometry", "Points", (isChecked)?1. : 0.);
				mCallback.onRequestRender();
			}
		});
		_listView.addItem("Display", showGeomPoints);
		CheckBox showGeomLines = new CheckBox(_listView.getContext());
		showGeomLines.setText("Show geometry lines");
		showGeomLines.setChecked((_gmsh.getDoubleOption("Geometry", "Lines") > 0.));
		showGeomLines.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				_gmsh.setDoubleOption("Geometry", "Lines", (isChecked)?1. : 0.);
				mCallback.onRequestRender();
			}
		});
		_listView.addItem("Display", showGeomLines);
		CheckBox showMeshSurfaceEdges = new CheckBox(_listView.getContext());
    	showMeshSurfaceEdges.setText("Show mesh surface edges");
    	showMeshSurfaceEdges.setChecked((_gmsh.getDoubleOption("Mesh", "SurfaceEdges") > 0.));
    	showMeshSurfaceEdges.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				_gmsh.setDoubleOption("Mesh", "SurfaceEdges", (isChecked)?1. : 0.);
				mCallback.onRequestRender();
			}
		});
    	_listView.addItem("Display", showMeshSurfaceEdges);
    	CheckBox showMeshVolumesEdges = new CheckBox(_listView.getContext());
    	showMeshVolumesEdges.setText("Show mesh volume edges");
    	showMeshVolumesEdges.setChecked((_gmsh.getDoubleOption("Mesh", "VolumeEdges") > 0.));
    	showMeshVolumesEdges.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				_gmsh.setDoubleOption("Mesh", "VolumeEdges", (isChecked)?1. : 0.);
				mCallback.onRequestRender();
			}
		});
    	_listView.addItem("Display", showMeshVolumesEdges);
    	this.refresh();
		return _listView;
	}
	
	public void refresh() {
		if(_gmsh == null) return;
		String[] PViews = _gmsh.getPView();
		for(int i=_listView.itemsCountInSection("Result"); i < PViews.length;i++){
			String[] infos = PViews[i].split("\n"); // name / IntervalsType (1=Iso 2=Continous 3=Discrete 4=Numeric)
			final int myID = i;
        	CheckBox checkbox = new CheckBox(_listView.getContext());
        	checkbox.setText(infos[0]);
        	checkbox.setChecked(infos[2].equals("1"));
        	checkbox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
				
				public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
					_gmsh.setPView(myID, -1, (isChecked)? 1 : 0, -1);
					mCallback.onRequestRender();
				}
			});
			_listView.addItem("Result", checkbox);
		}
	}
	
	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);
		try {
            mCallback = (OnOptionRequestRender) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString()
                    + " must implement OnHeadlineSelectedListener");
        }

	}
	
	private OnOptionRequestRender mCallback;
	public interface OnOptionRequestRender {
		public void onRequestRender();
	}
}