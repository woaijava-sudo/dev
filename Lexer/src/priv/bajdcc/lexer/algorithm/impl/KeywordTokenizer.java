package priv.bajdcc.lexer.algorithm.impl;

import java.util.HashMap;

import priv.bajdcc.lexer.algorithm.TokenAlgorithm;
import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.token.KeywordType;
import priv.bajdcc.lexer.token.Token;
import priv.bajdcc.lexer.token.TokenType;

/**
 * �ؼ��ֽ���
 * 
 * @author bajdcc
 *
 */
public class KeywordTokenizer extends TokenAlgorithm {

	/**
	 * �ؼ��ֹ�ϣ��
	 */
	private HashMap<String, KeywordType> m_HashKeyword = new HashMap<String, KeywordType>();

	public KeywordTokenizer() throws RegexException {
		super(getRegexString(), null);
		initializeHashMap();
	}

	public static String getRegexString() {
		StringBuilder sb = new StringBuilder();
		for (KeywordType keyword : KeywordType.values()) {// �ؼ���
			sb.append(keyword.getName() + "|");
		}
		sb.deleteCharAt(sb.length() - 1);
		return sb.toString();
	}

	@Override
	public boolean getGreedMode() {
		return true;
	}

	/**
	 * ��ʼ���ؼ��ֹ�ϣ��
	 */
	private void initializeHashMap() {
		for (KeywordType keyword : KeywordType.values()) {// �ؼ���
			m_HashKeyword.put(keyword.getName(), keyword);
		}
	}

	/*
	 * ���� Javadoc��
	 * 
	 * @see
	 * priv.bajdcc.lexer.algorithm.ITokenAlgorithm#getToken(java.lang.String,
	 * priv.bajdcc.lexer.token.Token)
	 */
	@Override
	public Token getToken(String string, Token token) {
		token.m_kToken = TokenType.KEYWORD;
		token.m_Object = m_HashKeyword.get(string);
		return token;
	}
}
