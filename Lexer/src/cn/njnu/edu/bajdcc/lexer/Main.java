package cn.njnu.edu.bajdcc.lexer;

import java.io.BufferedReader;
import java.io.FileReader;

public class Main {

	public static void main(String[] args) {
		String filename = "R:/新建文件夹/vmm.cpp";
		try {
			// 读文件
			BufferedReader br = new BufferedReader(new FileReader(filename));
			String line = "";
			StringBuffer sb = new StringBuffer();
			while ((line = br.readLine()) != null) {
				sb.append(line + System.getProperty("line.separator"));
			}
			String context = sb.toString();
			// 词法分析
			Lexer lexer = new Lexer(context);
			br.close();
		} catch (Exception e) {
			System.err.print(e.getMessage());
			e.printStackTrace();
		}
	}
}
