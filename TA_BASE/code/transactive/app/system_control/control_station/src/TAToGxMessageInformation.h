#pragma once

// Structure for a GX target command message
struct TAToGxMessageInformation
{
    CString targetSessionID;
    CString targetSchematicID;
    CString sourceEntityName;
    CString message;
};
