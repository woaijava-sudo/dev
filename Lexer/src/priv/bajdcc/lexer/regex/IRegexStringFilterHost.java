package priv.bajdcc.lexer.regex;

import priv.bajdcc.lexer.algorithm.ITokenAlgorithm;

/**
 * �ַ�����������
 * 
 * @author bajdcc
 *
 */
public interface IRegexStringFilterHost {
	/**
	 * �����ַ�ת���㷨
	 * @param alg �ַ�ת���㷨
	 */
	public void setFilter(ITokenAlgorithm alg);
}
