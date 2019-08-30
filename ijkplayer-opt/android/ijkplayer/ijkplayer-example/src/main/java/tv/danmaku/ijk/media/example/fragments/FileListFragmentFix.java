/*
 * Copyright (C) 2015 Bilibili
 * Copyright (C) 2015 Zhang Rui <bbcallen@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package tv.danmaku.ijk.media.example.fragments;

import android.app.Activity;
import android.content.Context;
import android.database.Cursor;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.Loader;
import android.support.v4.widget.SimpleCursorAdapter;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import tv.danmaku.ijk.media.example.R;
import tv.danmaku.ijk.media.example.content.PathCursor;
import tv.danmaku.ijk.media.example.content.PathCursorLoader;
import tv.danmaku.ijk.media.example.eventbus.FileExplorerEvents;

public class FileListFragmentFix extends Fragment {
    private static final String ARG_PATH = "path";

    private TextView mPathView;
    private ListView mFileListView;
    private SimpleAdapter mAdapter;
    public static FileListFragmentFix newInstance(String path) {
        FileListFragmentFix f = new FileListFragmentFix();

        // Supply index input as an argument.
        Bundle args = new Bundle();
        args.putString(ARG_PATH, path);
        f.setArguments(args);

        return f;
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        ViewGroup viewGroup = (ViewGroup) inflater.inflate(R.layout.fragment_file_list, container, false);

        initViews(viewGroup);
        return viewGroup;
    }

    private void initViews(View viewGroup){
        mPathView = (TextView) viewGroup.findViewById(R.id.path_view);
        mFileListView = (ListView) viewGroup.findViewById(R.id.file_list_view);

        mPathView.setVisibility(View.VISIBLE);

        final ArrayList <Map<String,Object>> lists = new ArrayList<>();
        Map<String,Object> map =new HashMap<>();
        map.put("image",R.drawable.ic_theme_play_arrow);
        map.put("content","/sdcard/DCIM/Camera/test.mp4");
        lists.add(map);
        Map<String,Object> map2 =new HashMap<>();
        map2.put("image",R.drawable.ic_theme_play_arrow);
        map2.put("content","/sdcard/DCIM/insta360_1080p_25M.mp4");
        lists.add(map2);
        Activity activity = getActivity();
        mAdapter = new SimpleAdapter(activity,lists,R.layout.fragment_file_list_item
                ,new String[]{"image","content"}
                ,new int[]{R.id.icon,R.id.name});
        mFileListView.setAdapter(mAdapter);
        mFileListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, final int position, final long id) {
                Map<String, Object> mMap = (Map<String, Object>) mAdapter.getItem(position);
                String path = (String) mMap.get("content");
                if (TextUtils.isEmpty(path))
                    return;
                FileExplorerEvents.getBus().post(new FileExplorerEvents.OnClickFile(path));
            }
        });
    }
    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
    }
}
