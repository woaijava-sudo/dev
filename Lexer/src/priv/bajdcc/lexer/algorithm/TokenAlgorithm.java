package priv.bajdcc.lexer.algorithm;

import java.util.HashMap;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.regex.IRegexStringFilter;
import priv.bajdcc.lexer.regex.IRegexStringIterator;
import priv.bajdcc.lexer.regex.IRegexStringAttribute;
import priv.bajdcc.lexer.regex.Regex;
import priv.bajdcc.lexer.token.MetaType;
import priv.bajdcc.lexer.token.Token;
import priv.bajdcc.lexer.token.TokenType;

/**
 * ����ʶ���㷨
 * 
 * @author bajdcc
 *
 */
/**
 * @author bajdcc
 *
 */
public abstract class TokenAlgorithm implements ITokenAlgorithm,
		IRegexStringAttribute {

	/**
	 * ����ƥ���������ʽ
	 */
	protected Regex m_Regex = null;

	/**
	 * ƥ����
	 */
	protected String m_strMatch = "";

	/**
	 * �ַ����˽ӿ�
	 */
	protected IRegexStringFilter m_Filter = null;

	/**
	 * �ַ����͹��α�
	 */
	protected HashMap<Character, MetaType> m_MetaMap = new HashMap<Character, MetaType>();

	public TokenAlgorithm(String regex, IRegexStringFilter filter)
			throws RegexException {
		m_Regex = new Regex(regex);
		if (filter != null) {
			m_Filter = filter;
			m_Regex.setFilter(m_Filter);
			MetaType[] metaTypes = m_Filter.getFilterMeta().getMetaTypes();
			for (int i = 0; i < metaTypes.length; i++) {
				m_MetaMap.put(metaTypes[i].getChar(), metaTypes[i]);
			}
		}
	}

	@Override
	public boolean accept(IRegexStringIterator iterator, Token token) {
		if (!iterator.available()) {
			token.m_kToken = TokenType.EOF;
			return true;
		}
		token.m_Position = iterator.position();
		if (m_Regex.match(iterator, this)) {// ƥ��ɹ�
			token = getToken(m_strMatch, token);// �Զ�ת������
			return true;
		}
		return false;
	}

	/*
	 * ���� Javadoc��
	 * 
	 * @see priv.bajdcc.lexer.regex.IRegexStringAttribute#getGreedMode()
	 */
	@Override
	public boolean getGreedMode() {
		return false;// Ĭ��Ϊ��̰��ģʽ
	}

	/*
	 * ���� Javadoc��
	 * 
	 * @see priv.bajdcc.lexer.algorithm.ITokenAlgorithm#getRegexStringFilter()
	 */
	@Override
	public IRegexStringFilter getRegexStringFilter() {
		return m_Filter;
	}

	/*
	 * ���� Javadoc��
	 * 
	 * @see priv.bajdcc.lexer.algorithm.ITokenAlgorithm#getMetaHash()
	 */
	@Override
	public HashMap<Character, MetaType> getMetaHash() {
		return m_MetaMap;
	}

	/*
	 * ���� Javadoc��
	 * 
	 * @see
	 * priv.bajdcc.lexer.regex.IRegexStringResult#setResult(java.lang.String)
	 */
	@Override
	public void setResult(String result) {
		m_strMatch = result;
	}
}
