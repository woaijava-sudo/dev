package priv.bajdcc.lexer.regex;

/**
 * �ַ������˽ӿ�
 * 
 * @author bajdcc
 *
 */
public interface IRegexStringFilter {
	
	/**
	 * ����
	 */
	public RegexStringIteratorData filter(IRegexStringIterator iterator);
	
	
	/**
	 * �������͹��˽ӿ�
	 */
	public IRegexStringFilterMeta getFilterMeta();
}
