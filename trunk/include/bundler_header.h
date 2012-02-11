

const char BundlerIdent[6] = { 'B', 'U', 'N', 'D', '0', '1' };

struct BundlerHeader
{
	char	ident[6];
	int		launchersize;
	int		zipsize;
};