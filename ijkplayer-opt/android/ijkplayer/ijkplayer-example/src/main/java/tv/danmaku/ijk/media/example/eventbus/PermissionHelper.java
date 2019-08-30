package tv.danmaku.ijk.media.example.eventbus;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;

import tv.danmaku.ijk.media.example.R;

/**
 * Android 6.0+ 权限申请助手
 *
 */
public class PermissionHelper {

	private static final String TAG = "PermissionHelper";

	/**
	 * 小tips:这里的int数值不能太大，否则不会弹出请求权限提示，测试的时候,改到1000就不会弹出请求了
	 */
	public final static int WRITE_EXTERNAL_STORAGE_CODE = 0x66;
	
	public final static int ACCESS_COARSE_LOCATION_CODE = 0x67;
	
	public final static int RECORD_AUDIO_CODE = 0x68;
	public final static int ACCESS_CAMERA_CODE = 0x69;
	
	private final static int REQUEST_OPEN_APPLICATION_SETTINGS_CODE = 0x123400;
	/**
	 * 有米 Android SDK 所需要向用户申请的权限列表
	 */
	private PermissionModel[] mPermissionModels = new PermissionModel[] {
			new PermissionModel(R.string.permission_location_title, Manifest.permission.ACCESS_COARSE_LOCATION,
					R.string.permission_location_explain,ACCESS_COARSE_LOCATION_CODE),
			new PermissionModel(R.string.permission_store_title, Manifest.permission.WRITE_EXTERNAL_STORAGE, 
					R.string.permission_store_explain,WRITE_EXTERNAL_STORAGE_CODE),
			new PermissionModel(R.string.permission_record_title, Manifest.permission.RECORD_AUDIO, 
					R.string.permission_record_explain,RECORD_AUDIO_CODE),
			new PermissionModel(R.string.permission_camera_title, Manifest.permission.CAMERA, 
					R.string.permission_camera_explain,ACCESS_CAMERA_CODE)};

	private Activity mActivity;

	private OnApplyPermissionListener mOnApplyPermissionListener;

	public PermissionHelper(Activity activity) {
		mActivity = activity;
	}

	public void setOnApplyPermissionListener(OnApplyPermissionListener onApplyPermissionListener) {
		mOnApplyPermissionListener = onApplyPermissionListener;
	}

	private PermissionModel findPermissionModel(int code){
		for (PermissionModel pm : mPermissionModels){
			if (pm.requestCode == code){
				return pm;
			}
		}
		return null;
	}
	/**
	 * 这里我们演示如何在Android 6.0+上运行时申请权限
	 */
	public void applyPermissions(int code) {
		PermissionModel findModel = findPermissionModel(code);
		if (findModel == null)return;
		try {
			if (PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(mActivity,
					findModel.permission)) {
				ActivityCompat.requestPermissions(mActivity, new String[] { findModel.permission }, findModel.requestCode);
				return;
			}
			if (mOnApplyPermissionListener != null) {
				mOnApplyPermissionListener.onAfterApplyAllPermission(code);
			}
		} catch (Throwable e) {
			Log.e(TAG, "", e);
		}
		return;
	}

	/**
	 * 对应Activity的 {@code onRequestPermissionsResult(...)} 方法
	 *
	 * @param requestCode
	 * @param permissions
	 * @param grantResults
	 */
	public void onRequestPermissionsResult(final int requestCode, String[] permissions, int[] grantResults) {
		switch (requestCode) {
		case ACCESS_COARSE_LOCATION_CODE:
		case RECORD_AUDIO_CODE:
		case WRITE_EXTERNAL_STORAGE_CODE:
		case ACCESS_CAMERA_CODE:
			// 如果用户不允许，我们视情况发起二次请求或者引导用户到应用页面手动打开
			if (PackageManager.PERMISSION_GRANTED != grantResults[0]) {

				// 二次请求，表现为：以前请求过这个权限，但是用户拒接了
				// 在二次请求的时候，会有一个“不再提示的”checkbox
				// 因此这里需要给用户解释一下我们为什么需要这个权限，否则用户可能会永久不在激活这个申请
				// 方便用户理解我们为什么需要这个权限
				if (ActivityCompat.shouldShowRequestPermissionRationale(mActivity, permissions[0])) {
					AlertDialog.Builder builder = new AlertDialog.Builder(mActivity).setTitle(R.string.permission_apply_tip)
							.setMessage(findPermissionExplain(permissions[0]))
							.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog, int which) {
									applyPermissions(requestCode);
								}
							});
					builder.setCancelable(false);
					builder.show();
				}
				// 到这里就表示已经是第3+次请求，而且此时用户已经永久拒绝了，这个时候，我们引导用户到应用权限页面，让用户自己手动打开
				else {
					String prompt = mActivity.getResources().getString(R.string.permission_apply_prompt);
					int pstrID = findPermissionName(permissions[0]);
					String pstr = "";
					if (pstrID != 0){
						pstr = mActivity.getResources().getString(pstrID);
					}
					prompt = String.format(prompt, pstr);
					AlertDialog.Builder builder = new AlertDialog.Builder(mActivity).setTitle(R.string.permission_apply_tip)
							.setMessage(prompt)
							.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog, int which) {
									openApplicationSettings(REQUEST_OPEN_APPLICATION_SETTINGS_CODE | requestCode);
								}
							}).setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog, int which) {
									mActivity.finish();
								}
							});
					builder.setCancelable(false);
					builder.show();
				}
				return;
			}

			// 到这里就表示用户允许了本次请求，我们继续检查是否还有待申请的权限没有申请
			if (isRequestedPermissionGranted(requestCode)) {
				if (mOnApplyPermissionListener != null) {
					mOnApplyPermissionListener.onAfterApplyAllPermission(requestCode);
				}
			} else {
				applyPermissions(requestCode);
			}
			break;
		}
	}

	/**
	 * 对应Activity的 {@code onActivityResult(...)} 方法
	 *
	 * @param requestCode
	 * @param resultCode
	 * @param data
	 */
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode & REQUEST_OPEN_APPLICATION_SETTINGS_CODE) {
		case REQUEST_OPEN_APPLICATION_SETTINGS_CODE:
			int code = requestCode & 0x000000ff;
			if (isRequestedPermissionGranted(code)) {
				if (mOnApplyPermissionListener != null) {
					mOnApplyPermissionListener.onAfterApplyAllPermission(requestCode);
				}
			} else {
				//mActivity.finish();
			}
			break;
		}
	}

	/**
	 * 判断是否所有的权限都被授权了
	 *
	 * @return
	 */
	public boolean isAllRequestedPermissionGranted() {
		for (PermissionModel model : mPermissionModels) {
			if (PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(mActivity, model.permission)) {
				return false;
			}
		}
		return true;
	}
	
	private boolean isRequestedPermissionGranted(int code) {
		PermissionModel findModel = findPermissionModel(code);
		if (findModel == null)return false;
		if (PackageManager.PERMISSION_GRANTED != ContextCompat.checkSelfPermission(mActivity, findModel.permission)) {
			return false;
		}
		return true;
	}

	/**
	 * 打开应用设置界面
	 *
	 * @param requestCode
	 *            请求码
	 *
	 * @return
	 */
	private boolean openApplicationSettings(int requestCode) {
		try {
			Intent intent = new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS,
					Uri.parse("package:" + mActivity.getPackageName()));
			intent.addCategory(Intent.CATEGORY_DEFAULT);

			// Android L 之后Activity的启动模式发生了一些变化
			// 如果用了下面的 Intent.FLAG_ACTIVITY_NEW_TASK ，并且是 startActivityForResult
			// 那么会在打开新的activity的时候就会立即回调 onActivityResult
			// intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			mActivity.startActivityForResult(intent, requestCode);
			return true;
		} catch (Throwable e) {
			Log.e(TAG, "", e);
		}
		return false;
	}

	/**
	 * 查找申请权限的解释短语
	 *
	 * @param permission
	 *            权限
	 *
	 * @return
	 */
	private int findPermissionExplain(String permission) {
		if (mPermissionModels != null) {
			for (PermissionModel model : mPermissionModels) {
				if (model != null && model.permission != null && model.permission.equals(permission)) {
					return model.explainID;
				}
			}
		}
		return 0;
	}

	/**
	 * 查找申请权限的名称
	 *
	 * @param permission
	 *            权限
	 *
	 * @return
	 */
	private int findPermissionName(String permission) {
		if (mPermissionModels != null) {
			for (PermissionModel model : mPermissionModels) {
				if (model != null && model.permission != null && model.permission.equals(permission)) {
					return model.nameID;
				}
			}
		}
		return 0;
	}

	private static class PermissionModel {

		/**
		 * 权限名称
		 */
		public int nameID;

		/**
		 * 请求的权限
		 */
		public String permission;

		/**
		 * 解析为什么请求这个权限
		 */
		public int explainID;

		/**
		 * 请求代码
		 */
		public int requestCode;

		public PermissionModel(int nameID, String permission, int explainID, int requestCode) {
			this.nameID = nameID;
			this.permission = permission;
			this.explainID = explainID;
			this.requestCode = requestCode;
		}
	}

	/**
	 * 权限申请事件监听
	 */
	public interface OnApplyPermissionListener {

		/**
		 * 申请所有权限之后的逻辑
		 */
		void onAfterApplyAllPermission(int code);
	}

}
