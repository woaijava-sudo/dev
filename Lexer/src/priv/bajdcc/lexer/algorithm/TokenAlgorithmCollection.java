package priv.bajdcc.lexer.algorithm;

import java.util.ArrayList;

import priv.bajdcc.lexer.error.IErrorHandler;
import priv.bajdcc.lexer.regex.IRegexStringFilterHost;
import priv.bajdcc.lexer.regex.IRegexStringIterator;
import priv.bajdcc.lexer.token.Token;
import priv.bajdcc.lexer.token.TokenType;

/**
 * ���ڳ�ȡ���ʵ��㷨���ϣ��������֡��ַ����ȣ�
 * 
 * @author bajdcc
 */
public class TokenAlgorithmCollection {
	/**
	 * �㷨����
	 */
	private ArrayList<ITokenAlgorithm> m_arrAlgorithms = new ArrayList<ITokenAlgorithm>();

	/**
	 * �ַ���������
	 */
	private IRegexStringIterator m_Iterator = null;

	/**
	 * �ַ�ת������
	 */
	private IRegexStringFilterHost m_FilterHost = null;
	
	/**
	 * ������
	 */
	private IErrorHandler m_Handler = null;

	public TokenAlgorithmCollection(IRegexStringIterator iterator,
			IRegexStringFilterHost host) {
		m_Iterator = iterator;
		m_FilterHost = host;
		m_Handler = new TokenErrorAdvanceHandler(iterator);
	}

	public void attach(ITokenAlgorithm alg) {
		m_arrAlgorithms.add(alg);
	}

	public void detach(ITokenAlgorithm alg) {
		m_arrAlgorithms.remove(alg);
	}

	public Token scan() {
		Token token = new Token();
		token.m_kToken = TokenType.ERROR;
		if (!m_Iterator.available()) {
			token.m_kToken = TokenType.EOF;
		} else {
			for (ITokenAlgorithm alg : m_arrAlgorithms) {
				m_FilterHost.setFilter(alg);
				m_Iterator.translate();
				if (alg.accept(m_Iterator, token))
					return token;
			}
			m_Handler.handleError();
		}
		return token;
	}
}
