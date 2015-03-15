package priv.bajdcc.lexer.token;

import java.util.ArrayList;
import java.util.List;

import priv.bajdcc.lexer.algorithm.ITokenVisitor;
import priv.bajdcc.lexer.algorithm.TokenAlgorithmCollection;

/**
 * �ʷ�������
 * 
 * @author bajdcc
 */
public class Lexer implements ITokenVisitor {
	/**
	 * ��������
	 */
	private ArrayList<Token> m_arrTokens = new ArrayList<Token>();

	/**
	 * ��ǰ����
	 */
	private Token m_Token;

	/**
	 * Դ�ַ���
	 */
	private String m_strContext;

	/**
	 * �㷨���ϣ�DFA��ʽ��
	 */
	private TokenAlgorithmCollection m_algCollections;

	public Lexer(String strInput) {
		m_strContext = strInput;
		analysis();
	}

	/**
	 * @return ��������
	 */
	public List<Token> getTokens() {
		return m_arrTokens;
	}

	/**
	 * ���дʷ�����
	 */
	private void analysis() {

	}
}
