package priv.bajdcc.lexer.token;

import java.util.ArrayList;
import java.util.List;

import priv.bajdcc.lexer.algorithm.ITokenVisitor;
import priv.bajdcc.lexer.algorithm.TokenAlgorithmCollection;

/**
 * 词法分析器
 * 
 * @author bajdcc
 */
public class Lexer implements ITokenVisitor {
	/**
	 * 单词数组
	 */
	private ArrayList<Token> arrTokens = new ArrayList<Token>();

	/**
	 * 当前单词
	 */
	private Token curToken;

	/**
	 * 源字符串
	 */
	private String strContext;

	/**
	 * 算法集合（DFA方式）
	 */
	private TokenAlgorithmCollection algCollections;

	public Lexer(String strInput) {
		strContext = strInput;
		analysis();
	}

	/**
	 * @return 单词数组
	 */
	public List<Token> getTokens() {
		return arrTokens;
	}

	/**
	 * 进行词法分析
	 */
	private void analysis() {

	}
}
