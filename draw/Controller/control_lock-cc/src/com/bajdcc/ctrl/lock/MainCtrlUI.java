package com.bajdcc.ctrl.lock;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ConnectException;
import java.net.Socket;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainCtrlUI extends Activity {
	private EditText editHost = null;
	private EditText editPort = null;
	private Button btnExit = null;
	private Button btnConn = null;
	private TextView txvConn = null;
	private EditText txvMsg = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main_ctrl_ui);
		
		editHost = (EditText)findViewById(R.id.edit_IP);
		editPort = (EditText)findViewById(R.id.edit_PORT);
		btnExit = (Button)findViewById(R.id.button_exit);
		btnConn = (Button)findViewById(R.id.button_conn);
		txvConn = (TextView)findViewById(R.id.textView_status);
		txvMsg = (EditText)findViewById(R.id.editText_msg);
		
		btnExit.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				System.exit(0);
			}
		});
		
		btnConn.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_IR));
			}
		});
		
		((Button)findViewById(R.id.Button00)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_00));
			}
		});
		
		((Button)findViewById(R.id.Button01)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_01));
			}
		});
		
		((Button)findViewById(R.id.Button02)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_02));
			}
		});
		
		((Button)findViewById(R.id.Button03)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_03));
			}
		});
		
		((Button)findViewById(R.id.Button04)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_04));
			}
		});
		
		((Button)findViewById(R.id.Button05)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_05));
			}
		});
		
		((Button)findViewById(R.id.Button06)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_06));
			}
		});
		
		((Button)findViewById(R.id.Button07)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_07));
			}
		});
		
		((Button)findViewById(R.id.ButtonF1)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_F1));
			}
		});
		
		((Button)findViewById(R.id.ButtonF3)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_F3));
			}
		});
		
		((Button)findViewById(R.id.ButtonF6)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_F6));
			}
		});
		
		((Button)findViewById(R.id.Button_d1)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_d1));
			}
		});
		
		((Button)findViewById(R.id.Button_d2)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_d2));
			}
		});
		
		((Button)findViewById(R.id.Button_d3)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_d3));
			}
		});
		
		((Button)findViewById(R.id.Button_d4)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_d4));
			}
		});
		
		((Button)findViewById(R.id.Button_lock)).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SendMessage(getString(R.string.msg_ST));
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main_ctrl_ui, menu);
		return true;
	}
	
	public void SendMessage(String strSend) {
		String strHost = editHost.getText().toString();
		int port = Integer.parseInt(editPort.getText().toString());
		
		
		try {			
			Socket socket = new Socket(strHost, port);
			socket.setSoTimeout(3000);
			OutputStream outputStream = socket.getOutputStream();
			InputStream inputStream = socket.getInputStream();
			outputStream.write(strSend.getBytes());
			byte[] buffer = new byte[255];
			inputStream.read(buffer);
			String strRecv = new String(buffer);
			txvMsg.setText(strRecv);
			socket.close();
			txvConn.setText(R.string.static_str_conn);
		} catch (ConnectException e) {
			Toast.makeText(getApplicationContext(),
					"连接错误！", Toast.LENGTH_SHORT).show();
			txvConn.setText(R.string.static_str_not_conn);
			txvMsg.setText(R.string.static_str_no_status);
			e.printStackTrace();
		} catch (IOException e) {
			Toast.makeText(getApplicationContext(),
					"I/O 错误！", Toast.LENGTH_SHORT).show();
			txvConn.setText(R.string.static_str_not_conn);
			txvMsg.setText(R.string.static_str_no_status);
			e.printStackTrace();
		}
	}
}
