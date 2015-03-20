package priv.bajdcc.lexer.test;

import java.util.Scanner;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.regex.Regex;

public class TestRegex {

	public static void main(String[] args) {
		try {
			Scanner scanner = new Scanner(System.in);
			String str = scanner.nextLine();
			scanner.close();
			Regex ra = new Regex(str, true);
		} catch (RegexException e) {
			System.err.println(e.getLocation() + "," + e.getMessage());
			e.printStackTrace();
		}
	}
}
