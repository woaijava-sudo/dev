package priv.bajdcc.lexer.regex;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;
import priv.bajdcc.lexer.token.MetaType;

/**
 * �ַ����������ӿ�
 * @author bajdcc
 *
 */
public interface IRegexStringIterator {

	/**
	 * �׳�����
	 * 
	 * @param error
	 *            ��������
	 * @throws RegexException
	 */
	public abstract void err(RegexError error) throws RegexException;

	/**
	 * ������һ���ַ�
	 */
	public abstract void next();

	/**
	 * �жϵ�ǰλ�ò���ĩβ
	 * 
	 * @return ��ǰ�ַ��Ƿ���Ч
	 */
	public abstract boolean available();

	/**
	 * ǰ��һ���ַ���look forward��
	 * 
	 */
	public abstract void advance();

	/**
	 * ��õ�ǰ�ַ�
	 * 
	 * @return ��ǰ�ַ�
	 */
	public abstract char current();

	/**
	 * ȷ�ϵ�ǰ�ַ�
	 * 
	 * @param meta
	 *            ����
	 * @param error
	 *            �׳��Ĵ���
	 * @throws RegexException
	 */
	public abstract void expect(MetaType meta, RegexError error)
			throws RegexException;
}