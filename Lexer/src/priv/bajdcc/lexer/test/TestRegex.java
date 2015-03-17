package priv.bajdcc.lexer.test;

import java.util.Scanner;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.regex.RegexAnalysis;

public class TestRegex {

	public static void main(String[] args) {
		try {
			Scanner scanner = new Scanner(System.in);
			String str = scanner.nextLine();
			scanner.close();
			RegexAnalysis ra = new RegexAnalysis(str);
			System.out.println(ra);
			System.out.println(ra.getStatusString());
		} catch (RegexException e) {
			System.err.println(e.getLocation() + "," + e.getMessage());
			e.printStackTrace();
		}
	}
}
