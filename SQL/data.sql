-- =================================================================
-- Sample Data for Project Templates
-- =================================================================

-- Note: The content in these INSERT statements should be a single line.
-- C++ string literals in the original generator were multi-line for readability,
-- but here they are represented as a single string with newline characters (\n).

-- Template for LICENSE file
INSERT INTO templates (project_name, file_path, content) VALUES (
    'generic',
    'LICENSE',
    'MIT License\n\nCopyright (c) 2024 {{author}}\n\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files (the "Software"), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions:\n\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.'
);

-- Template for README.md file
INSERT INTO templates (project_name, file_path, content) VALUES (
    'generic',
    'README.md',
    '# {{project_name}}\n\n## Description\n{{description}}\n\n## Goal\n{{goal}}\n\n## Building\n\n### Using Makefile\n```bash\nmake\n```\n\n## Author\n{{author}}\n'
);

-- =================================================================
-- End of Sample Data
-- =================================================================
