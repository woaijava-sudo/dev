package priv.bajdcc.lexer.algorithm;

import java.util.HashMap;

import priv.bajdcc.lexer.regex.IRegexStringFilter;
import priv.bajdcc.lexer.regex.IRegexStringIterator;
import priv.bajdcc.lexer.token.MetaType;
import priv.bajdcc.lexer.token.Token;

/**
 * ���/��ȡ���ʵ��㷨�ӿ�
 * 
 * @author bajdcc
 */
public interface ITokenAlgorithm {

	/**
	 * ��ǰ�㷨�Ƿ������ӦVisitor���󣨼��Ƿ�ƥ�䣩
	 * 
	 * @param iterator
	 *            �ַ�����������
	 * @param token
	 *            ���صĵ��ʣ����ܳ�������ΪEOL��EOF��
	 * @return �㷨ƥ����
	 */
	boolean accept(IRegexStringIterator iterator, Token token);

	/**
	 * �����ַ����������
	 */
	public abstract IRegexStringFilter getRegexStringFilter();

	/**
	 * �����ַ����͹�ϣӳ���
	 */
	public abstract HashMap<Character, MetaType> getMetaHash();

	/**
	 * ����������ʽ�ַ���
	 * 
	 * @param string
	 *            ƥ����ַ���
	 * @param token
	 *            ����ĵ���
	 * @return ����ĵ���
	 */
	public abstract Token getToken(String string, Token token);
}
